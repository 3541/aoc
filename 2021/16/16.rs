use std::mem::transmute;

use bitvec::prelude::*;

struct Packet {
    version: u8,
    node: Node,
}

#[repr(u8)]
#[allow(dead_code)]
enum Op {
    Sum = 0,
    Product = 1,
    Min = 2,
    Max = 3,
    Gt = 5,
    Lt = 6,
    Eq = 7,
}

impl Op {
    fn from(b: u8) -> Op {
        assert!(b < 8);
        unsafe { transmute(b) }
    }
}

enum Node {
    Literal(usize),
    Operator {
        operator: Op,
        operands: Vec<Box<Packet>>,
    },
}

impl Node {
    fn eval(&self) -> usize {
        match self {
            Node::Literal(n) => *n,
            Node::Operator { operator, operands } => {
                let operands: Vec<usize> = operands.iter().map(|p| p.eval()).collect();
                match operator {
                    Op::Sum => operands.iter().sum(),
                    Op::Product => operands.iter().product(),
                    Op::Min => *operands.iter().min().unwrap(),
                    Op::Max => *operands.iter().max().unwrap(),
                    Op::Lt => (operands[0] < operands[1]) as usize,
                    Op::Gt => (operands[0] > operands[1]) as usize,
                    Op::Eq => (operands[0] == operands[1]) as usize,
                }
            }
        }
    }
}

struct PacketIterator<'a>(Vec<&'a Packet>);

impl<'a> IntoIterator for &'a Packet {
    type Item = &'a Packet;
    type IntoIter = PacketIterator<'a>;

    fn into_iter(self) -> PacketIterator<'a> {
        PacketIterator(vec![self])
    }
}

impl<'a> Iterator for PacketIterator<'a> {
    type Item = &'a Packet;

    fn next(&mut self) -> Option<&'a Packet> {
        let ret = self.0.pop()?;
        match &ret.node {
            Node::Literal(_) => {}
            Node::Operator {
                operands: children, ..
            } => self.0.extend(children.iter().map(Box::as_ref)),
        }

        Some(ret)
    }
}

enum Length {
    Bits(usize),
    Count(usize),
}

impl Packet {
    fn version(packet: &BitSlice<Msb0>) -> u8 {
        packet[0..3].load_be()
    }

    fn ty(packet: &BitSlice<Msb0>) -> u8 {
        packet[3..6].load_be::<u8>()
    }

    fn is_literal(packet: &BitSlice<Msb0>) -> bool {
        Packet::ty(packet) == 4
    }

    fn payload_length(packet: &BitSlice<Msb0>) -> Length {
        assert!(!Self::is_literal(packet));
        if packet[6] {
            Length::Count(packet[7..][..11].load_be())
        } else {
            Length::Bits(packet[7..][..15].load_be())
        }
    }

    fn parse(packet: &BitSlice<Msb0>) -> Packet {
        fn parse_inner(packet: &BitSlice<Msb0>) -> (Packet, usize) {
            let mut size = 6;
            let version = Packet::version(packet);
            if Packet::is_literal(packet) {
                let mut num = 0;
                loop {
                    num *= 16;
                    num += packet[size + 1..][..4].load_be::<usize>();
                    if !packet[size] {
                        return (
                            Packet {
                                version,
                                node: Node::Literal(num),
                            },
                            size + 5,
                        );
                    }
                    size += 5;
                }
            } else {
                size += 1;
                let mut children = Vec::new();

                match Packet::payload_length(packet) {
                    Length::Count(n) => {
                        size += 11;
                        for _ in 0..n {
                            let (child, child_size) = parse_inner(&packet[size..]);
                            size += child_size;
                            children.push(Box::new(child));
                        }
                    }

                    Length::Bits(n) => {
                        size += 15;
                        let mut offset = 0;
                        while offset < n {
                            let (child, child_size) = parse_inner(&packet[size + offset..]);
                            offset += child_size;
                            children.push(Box::new(child));
                        }

                        size += offset;
                    }
                }

                (
                    Packet {
                        version,
                        node: Node::Operator {
                            operands: children,
                            operator: Op::from(Packet::ty(packet)),
                        },
                    },
                    size,
                )
            }
        }

        parse_inner(packet).0
    }

    fn eval(&self) -> usize {
        self.node.eval()
    }
}

fn main() {
    const INPUT: &'static [u8] = include_bytes!("data");

    let mut raw_packet: BitVec<Msb0> = BitVec::with_capacity(INPUT.len() * 4);
    raw_packet.resize(raw_packet.capacity(), false);
    for (i, d) in INPUT.iter().copied().enumerate() {
        if d == b'\n' {
            break;
        }
        raw_packet[i * 4..][..4].store(if d < b'A' { d - b'0' } else { d - b'A' + 10 });
    }

    let packet = Packet::parse(&raw_packet);
    let sum: usize = packet.into_iter().map(|p| p.version as usize).sum();
    println!("{}", sum);

    println!("{}", packet.eval());
}
