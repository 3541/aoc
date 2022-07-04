fn increases(i: &[usize]) -> usize {
    i.windows(2).map(|w| (w[1] > w[0]) as usize).sum()
}

fn main() {
    const INPUT: &'static str = include_str!("data");
    let input: Vec<usize> = INPUT.lines().flat_map(str::parse).collect();

    let part_1 = increases(&input);
    let part_2 = increases(&input.windows(3).map(|w| w.iter().sum()).collect::<Vec<usize>>());

    println!("Part 1: {}.\nPart 2: {}.", part_1, part_2);
}
