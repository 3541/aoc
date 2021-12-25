use std::cmp::{Eq, Ord, Ordering, PartialOrd, Reverse};
use std::collections::{BinaryHeap, HashMap};
use std::fmt::{self, Display};

const CELLS: [u8; 4] = [b'A', b'B', b'C', b'D'];
const HALL_VALID: [bool; 11] = [
    true, true, false, true, false, true, false, true, false, true, true,
];

#[derive(Eq, PartialEq, Hash, Clone)]
struct Pos {
    halls: [u8; 11],
    cells: [[u8; 4]; 4],
}

#[derive(Eq, PartialEq, Clone)]
struct State {
    cost: usize,
    pos: Pos,
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
        Reverse(self.cost).cmp(&Reverse(other.cost))
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Display for Pos {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "#############")?;
        write!(f, "#")?;
        for h in self.halls {
            write!(f, "{}", h as char)?;
        }
        writeln!(f, "#")?;
        write!(f, "###")?;
        for c in self.cells {
            write!(f, "{}#", c[0] as char)?;
        }
        writeln!(f, "##")?;

        for i in 1..self.cells[0].len() {
            write!(f, "  ")?;
            for c in self.cells {
                write!(f, "#{}", c[i] as char)?;
            }
            writeln!(f, "#")?;
        }

        write!(f, "  #########")
    }
}

impl Display for State {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "{}", self.cost)?;
        write!(f, "{}", self.pos)
    }
}

impl Pos {
    fn moves(&self) -> Vec<(Self, usize)> {
        fn cost(a: u8) -> usize {
            match a {
                b'A' => 1,
                b'B' => 10,
                b'C' => 100,
                _ => 1000,
            }
        }

        fn dest(a: u8) -> usize {
            (a - b'A') as usize
        }

        let mut ret = Vec::new();

        for (ci, cell) in self.cells.iter().enumerate() {
            let mut i = 0;
            while i < cell.len() && cell[i] == b'.' {
                i += 1;
            }

            // Cell is empty.
            if i == cell.len() {
                continue;
            }

            // Cell is all final.
            let ty = cell[i];
            if CELLS[ci] == ty && (i == 3 || cell[i + 1..].iter().all(|c| *c == CELLS[ci])) {
                continue;
            }

            let mut new_cell = cell.clone();
            new_cell[i] = b'.';
            let mut new_cells = self.cells.clone();
            new_cells[ci] = new_cell;
            let base_cost = (i + 1) * cost(ty);
            let hall_index = 2 + ci * 2;

            let mut find_spots = |diff: isize| {
                let mut index = hall_index;
                loop {
                    if self.halls[index] != b'.' {
                        break;
                    }

                    if HALL_VALID[index] {
                        let mut new_halls = self.halls.clone();
                        new_halls[index] = ty;
                        ret.push((
                            Pos {
                                halls: new_halls,
                                cells: new_cells,
                            },
                            base_cost
                                + (hall_index as isize - index as isize).unsigned_abs() * cost(ty),
                        ));
                    }

                    if index == 0 || index == self.halls.len() - 1 {
                        break;
                    }

                    index = (index as isize + diff) as usize;
                }
            };

            find_spots(-1);
            find_spots(1);
        }

        'halls: for (hi, &h) in self.halls.iter().enumerate() {
            if h == b'.' {
                continue;
            }

            let dest_cell = self.cells[dest(h)];
            if dest_cell.iter().any(|&c| c != b'.' && c != h) {
                continue;
            }
            let dest_index = dest_cell
                .iter()
                .enumerate()
                .rev()
                .find(|(_, c)| **c == b'.')
                .unwrap()
                .0;

            let dest_hi = dest(h) * 2 + 2;
            let sign = if hi < dest_hi { 1 } else { -1 };
            let mut index = (hi as isize + sign) as usize;
            while index != dest_hi {
                if self.halls[index] != b'.' {
                    continue 'halls;
                }

                index = (index as isize + sign) as usize;
            }

            let mut new_halls = self.halls.clone();
            new_halls[hi] = b'.';

            let mut new_cell = dest_cell.clone();
            new_cell[dest_index] = h;
            let mut new_cells = self.cells.clone();
            new_cells[dest(h)] = new_cell;

            ret.push((
                Pos {
                    halls: new_halls,
                    cells: new_cells,
                },
                (dest_index + 1 + (hi as isize - dest_hi as isize).unsigned_abs()) * cost(h),
            ));
        }

        ret
    }
}

impl State {
    fn complete(&self) -> bool {
        self.pos
            .cells
            .iter()
            .enumerate()
            .all(|(i, c)| c.iter().all(|v| *v == CELLS[i]))
    }
}

fn main() {
    let initial = State {
        cost: 0,
        pos: Pos {
            halls: [b'.'; 11],
            cells: [
                [b'A', b'D', b'D', b'D'],
                [b'C', b'C', b'B', b'D'],
                [b'B', b'B', b'A', b'B'],
                [b'A', b'A', b'C', b'C'],
            ],
        },
    };
    let mut states = BinaryHeap::from([initial.clone()]);
    let mut transitions = HashMap::from([(initial.pos.clone(), initial.pos.moves())]);

    let cost = loop {
        let current = states.pop().unwrap();
        print!("\r{}", current.cost);
        if current.complete() {
            break current.cost;
        }

        for (next, cost_delta) in transitions
            .entry(current.pos.clone())
            .or_insert_with(|| current.pos.moves())
        {
            states.push(State {
                pos: next.clone(),
                cost: current.cost + *cost_delta,
            })
        }
    };

    println!("\r{}", cost);
}
