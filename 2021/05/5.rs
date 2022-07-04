use std::collections::HashMap;
use std::error::Error;
use std::cmp::{min, max};

fn solve(input: &str, part2: bool) -> Result<usize, Box<dyn Error>> {
     fn parse_coord(s: &str) -> Option<(usize, usize)> {
        let l = s.split(',').flat_map(str::parse::<usize>).collect::<Vec<_>>();
        if l.len() != 2 {
            None
        } else {
            Some((l[0], l[1]))
        }
    }

    let grid = input.lines().try_fold(HashMap::new(), |mut grid, line| {
        let (start, end) = line.split_once(" -> ")?;
        let (start, end) = (parse_coord(start)?, parse_coord(end)?);

        let range: Vec<_> = if start.1 == end.1 {
            (min(start.0, end.0)..=max(start.0, end.0)).map(|n| (n, start.1)).collect()
        } else if start.0 == end.0 {
            (min(start.1, end.1)..=max(start.1, end.1)).map(|n| (start.0, n)).collect()
        } else if !part2 {
            Vec::new()
        } else {
            let mut r = Vec::new();
            let (mut x, mut y) = start;
            let sign = ((end.0 as isize - start.0 as isize).signum(),
                        (end.1 as isize - start.1 as isize).signum());
            while (x, y) != end {
                r.push((x, y));
                x = (x as isize + sign.0) as usize;
                y = (y as isize + sign.1) as usize;
            }
            r.push(end);

            r
        };
        for c in range {
            grid.entry(c).and_modify(|v| *v += 1).or_insert(1);
        }

        Some(grid)
    }).ok_or("Failed to build grid.")?;

    Ok(grid.values().filter(|&&v| v > 1).count())
}

fn main() -> Result<(), Box<dyn Error>> {
    const INPUT: &'static str = include_str!("data");

    println!("Part 1: {}.", solve(INPUT, false)?);
    println!("Part 2: {}.", solve(INPUT, true)?);

    Ok(())
}
