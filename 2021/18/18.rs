#![feature(int_roundings)]
#![feature(int_log)]

use std::borrow::Cow;
use std::cmp::max;
use std::ops::Add;

use itertools::Itertools;

#[derive(Clone)]
struct Number<'a>(Cow<'a, str>);

impl Number<'_> {
    fn new(v: impl Into<String>) -> Number<'static> {
        Number(Cow::Owned(v.into()))
    }

    fn new_ref<'a>(v: &'a str) -> Number<'a> {
        Number(Cow::Borrowed(v))
    }

    fn magnitude(&self) -> usize {
        if self.0.as_bytes()[0] != b'[' {
            (self.0.as_bytes()[0] - b'0') as usize
        } else {
            let mut i = 1;
            let mut c = 0;
            while self.0.as_bytes()[i] == b'[' || c > 0 {
                match self.0.as_bytes()[i] {
                    b'[' => c += 1,
                    b']' => c -= 1,
                    _ => {}
                }
                i += 1;
            }
            if i == 1 {
                i = 2;
            }

            3 * Number::new_ref(&self.0[1..i]).magnitude()
                + 2 * Number::new_ref(&self.0[i + 1..]).magnitude()
        }
    }

    fn values(&self) -> (usize, usize) {
        let comma = self.0.find(',').unwrap();
        let close = self.0.find(']').unwrap();
        (
            self.0[1..comma].parse().unwrap(),
            self.0[comma + 1..close].parse().unwrap(),
        )
    }
}

impl Number<'static> {
    fn reduce_explode(&mut self) -> bool {
        let mut depth = 0;
        for i in 0..self.0.len() {
            let c = self.0.as_bytes()[i];
            if c == b'[' {
                depth += 1;
            } else if c == b']' {
                depth -= 1;
            }

            if depth >= 5 {
                let close = i + self.0[i..].find(']').unwrap();
                let (left, right) = Number::new_ref(&self.0[i..=close]).values();

                let mut shift = 0usize;
                if let Some(n) = self.0[..i].rfind(char::is_numeric) {
                    let left_num_range = self.0[..=n]
                        .rfind(|c| !char::is_numeric(c))
                        .unwrap_or(n - 1)
                        + 1..=n;
                    let left_num = self.0[left_num_range.clone()].parse::<usize>().unwrap();
                    self.0
                        .to_mut()
                        .replace_range(left_num_range, &format!("{}", left_num + left));
                    shift = (max(left_num + left, 1).log10() - max(left_num, 1).log10()) as usize;
                }

                let close = close + shift;
                let i = i + shift;
                if let Some(n) = self.0[close..].find(char::is_numeric) {
                    let n = close + n;
                    let right_num_range =
                        n..n + self.0[n..].find(|c| !char::is_numeric(c)).unwrap_or(1);
                    let right_num = self.0[right_num_range.clone()].parse::<usize>().unwrap();
                    self.0
                        .to_mut()
                        .replace_range(right_num_range, &format!("{}", right_num + right));
                }

                self.0.to_mut().replace_range(i..=close, "0");

                return true;
            }
        }

        false
    }

    fn reduce_split(&mut self) -> bool {
        let mut i = 0;
        while i < self.0.len() {
            if !char::is_numeric(self.0.as_bytes()[i] as char) {
                i += 1;
                continue;
            }

            let num_range = i..i + self.0[i..].find(|c| !char::is_numeric(c)).unwrap_or(1);
            let num = self.0[num_range.clone()].parse::<usize>().unwrap();
            if num < 10 {
                i = num_range.end;
                continue;
            }

            self.0.to_mut().replace_range(
                num_range,
                &format!("[{},{}]", num / 2, num.unstable_div_ceil(2)),
            );
            return true;
        }

        false
    }

    fn reduce(&mut self) {
        loop {
            if !self.reduce_explode() && !self.reduce_split() {
                break;
            }
        }
    }
}

impl Add for Number<'_> {
    type Output = Number<'static>;

    fn add(self, rhs: Number) -> Self::Output {
        let mut ret = Number(Cow::Owned(format!("[{},{}]", self.0, rhs.0)));
        ret.reduce();
        ret
    }
}

fn main() {
    const INPUT: &'static str = include_str!("data");

    let numbers = INPUT.lines().map(Number::new);
    let sum = numbers
        .clone()
        .reduce(|a, n| a + n)
        .expect("Missing number.");
    println!("{}", sum.magnitude());

    let max_sum = numbers
        .permutations(2)
        .map(|mut p| p.pop().unwrap() + p.pop().unwrap())
        .map(|n| n.magnitude())
        .max()
        .expect("Missing number.");
    println!("{}", max_sum);
}
