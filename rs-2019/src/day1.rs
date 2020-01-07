pub fn solve() {
    let input = include_str!("../res/day1.txt");
    let modules: Vec<u32> = input.lines().map(parse_module).collect();
    
    let fuel_required: u32 = modules.iter().map(|m| calculate_fuel(m).unwrap()).sum();
    println!("Day1 Part1 answer = {}", fuel_required);

    let fuel_required: u32 = modules.iter().map(calculate_fuel_decaying).sum();
    println!("Day1 Part2 answer = {}", fuel_required);

}

fn parse_module(line: &str) -> u32 {
    line.parse().expect("Line was not an integer")
}

fn calculate_fuel(mass: &u32) -> Option<u32>  {
    (mass / 3).checked_sub(2)
}

use std::iter::successors;

fn calculate_fuel_decaying(mass: &u32) -> u32 {
    successors(Some(*mass), calculate_fuel).skip(1).sum()
}
