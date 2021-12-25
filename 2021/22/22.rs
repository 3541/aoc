use std::borrow::Cow;
use std::cmp::{max, min};
use std::error::Error;
use std::ops::RangeInclusive;

use regex::Regex;

#[derive(Debug, Clone)]
struct Prism {
    on: bool,
    x: RangeInclusive<isize>,
    y: RangeInclusive<isize>,
    z: RangeInclusive<isize>,
}

fn intersection(lhs: &RangeInclusive<isize>, rhs: &RangeInclusive<isize>) -> RangeInclusive<isize> {
    max(*lhs.start(), *rhs.start())..=min(*lhs.end(), *rhs.end())
}

impl Prism {
    fn intersects(&self, other: &Self) -> bool {
        self.x.start() <= other.x.end()
            && other.x.start() <= self.x.end()
            && self.y.start() <= other.y.end()
            && other.y.start() <= self.y.end()
            && self.z.start() <= other.z.end()
            && other.z.start() <= self.z.end()
    }

    fn intersection(&self, other: &Self) -> Option<Self> {
        if !self.intersects(other) {
            None
        } else {
            Some(Self {
                on: !other.on,
                x: intersection(&self.x, &other.x),
                y: intersection(&self.y, &other.y),
                z: intersection(&self.z, &other.z),
            })
        }
    }

    fn sign(&self) -> isize {
        if self.on {
            1
        } else {
            -1
        }
    }

    fn volume(&self) -> isize {
        (self.x.end() - self.x.start() + 1)
            * (self.y.end() - self.y.start() + 1)
            * (self.z.end() - self.z.start() + 1)
            * self.sign()
    }

    fn in_part1_range(&self) -> bool {
        [
            self.x.start(),
            self.x.end(),
            self.y.start(),
            self.y.end(),
            self.z.start(),
            self.z.end(),
        ]
        .into_iter()
        .all(|n| n.abs() <= 50)
    }
}

fn combine<'a>(prisms: impl Iterator<Item = &'a Prism>) -> isize {
    prisms
        .fold(Vec::<Cow<'a, Prism>>::new(), |mut resolved, prism| {
            resolved.append(
                &mut resolved
                    .iter()
                    .flat_map(|r| Some(Cow::Owned(prism.intersection(r)?)))
                    .collect(),
            );
            if prism.on {
                resolved.push(Cow::Borrowed(prism));
            }

            resolved
        })
        .iter()
        .map(|p| p.volume())
        .sum()
}

fn main() -> Result<(), Box<dyn Error>> {
    const INPUT: &'static str = include_str!("data");

    let pattern =
        Regex::new(r"(?m)^(on|off) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+)$")?;
    let prisms = pattern
        .captures_iter(INPUT)
        .flat_map(|c| {
            Result::<_, Box<dyn Error>>::Ok(Prism {
                on: &c[1] == "on",
                x: c[2].parse()?..=c[3].parse()?,
                y: c[4].parse()?..=c[5].parse()?,
                z: c[6].parse()?..=c[7].parse()?,
            })
        })
        .collect::<Vec<Prism>>();

    println!("{}", combine(prisms.iter().filter(|p| p.in_part1_range())));
    println!("{}", combine(prisms.iter()));

    Ok(())
}
