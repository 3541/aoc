use std::collections::HashMap;
use std::error::Error;

fn step(
    rules: &HashMap<(u8, u8), u8>,
    polymer: &HashMap<(u8, u8), usize>,
) -> HashMap<(u8, u8), usize> {
    polymer
        .iter()
        .fold(HashMap::new(), |mut poly, (&pair, &count)| {
            if let Some(&new) = rules.get(&pair) {
                *poly.entry((pair.0, new)).or_insert(0) += count;
                *poly.entry((new, pair.1)).or_insert(0) += count;
            } else {
                *poly.entry(pair).or_insert(0) += count;
            }

            poly
        })
}

fn run(
    rules: &HashMap<(u8, u8), u8>,
    polymer: &HashMap<(u8, u8), usize>,
    count: usize,
) -> HashMap<(u8, u8), usize> {
    let mut ret = polymer.clone();
    for _ in 0..count {
        ret = step(rules, &ret);
    }

    ret
}

fn value(polymer: &HashMap<(u8, u8), usize>, last: u8) -> Option<usize> {
    let freqs = polymer
        .iter()
        .fold(HashMap::from([(last, 1)]), |mut f, (pair, &count)| {
            *f.entry(pair.0).or_insert(0) += count;
            f
        });

    let max = freqs.iter().max_by_key(|(_, &f)| f)?.1;
    let min = freqs.iter().min_by_key(|(_, &f)| f)?.1;

    Some(max - min)
}

fn main() -> Result<(), Box<dyn Error>> {
    const INPUT: &'static str = include_str!("data");
    let mut lines = INPUT.lines();

    let polyline = lines.next().ok_or("Missing polymer.")?.as_bytes();
    let last = *polyline.last().unwrap();
    let polymer = polyline.windows(2).fold(HashMap::new(), |mut p, w| {
        *p.entry((w[0], w[1])).or_insert(0) += 1usize;
        p
    });
    lines.next(); // Blank line.

    let rules = lines
        .map(|s| s.split_at(2))
        .map(|(pat, new)| (pat.as_bytes(), *new.as_bytes().last().unwrap()))
        .fold(HashMap::new(), |mut r, (pat, new)| {
            r.insert((pat[0], pat[1]), new);
            r
        });

    println!(
        "{}",
        value(&run(&rules, &polymer, 10), last).ok_or("Missing frequency.")?
    );
    println!(
        "{}",
        value(&run(&rules, &polymer, 40), last).ok_or("Missing frequency.")?
    );

    Ok(())
}
