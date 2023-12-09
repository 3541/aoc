use std::env::args;
use std::error::Error;
use std::fs;

fn main() -> Result<(), Box<dyn Error>> {
    let data = fs::read_to_string(args().nth(1).unwrap_or(String::from("./data")))?;
    let top_count = str::parse::<usize>(&args().nth(2).unwrap_or(String::from("1")))?;

    let mut elves = data
        .split("\n\n")
        .map(|elf| elf.split('\n').flat_map(str::parse::<usize>).sum())
        .collect::<Vec<usize>>();
    elves.sort_by(|a, b| b.cmp(a));

    let result = elves[..top_count].iter().sum::<usize>();
    println!("{}", result);

    Ok(())
}
