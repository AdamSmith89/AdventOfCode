use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

pub fn run() -> i32 {    
    if let Ok(lines) = read_lines("res/day1pt1.txt") {
        let input: Vec<i32> = lines.map(|line_result| line_result.unwrap().parse().unwrap()).collect();
        return calc_pt1(input, 2020);
    }
    
    return -1;
}

fn calc_pt1(expense_report: Vec<i32>, sum_target: i32) -> i32 {
    let mut expense_report = expense_report;
    expense_report.sort();

    for entry in &expense_report {
        let rhs = sum_target - entry;

        if let Ok(_index) = expense_report.binary_search(&rhs) {
            return entry * rhs;
        }
    }

    return -1;
}

fn calc_pt2(expense_report: Vec<i32>, sum_target: i32) -> i32 {
    let mut expense_report = expense_report;
    expense_report.sort();

    for (index1, entry1) in expense_report.iter().enumerate() {
        for (index2, entry2) in expense_report.iter().skip(index1 + 1).enumerate() {
            for (index3, entry3 in expense_report)
        }
    }

    for entry in &expense_report {
        let rhs = sum_target - entry;

        if let Ok(_index) = expense_report.binary_search(&rhs) {
            return entry * rhs;
        }
    }

    return -1;
}

#[cfg(test)]
mod tests {
    // Note this useful idiom: importing names from outer (for mod tests) scope.
    use super::*;

    #[test]
    fn calc_pt1_simple_set() {
        assert_eq!(calc_pt1(vec!{1, 2, 3}, 5), 6);
    }

    #[test]
    fn calc_pt1_aoc_example() {
        assert_eq!(calc_pt1(vec!{1721, 979, 366, 299, 675, 1456}, 2020), 514579);
    }

    #[test]
    fn calc_pt2_simple_set() {
        assert_eq!(calc_pt1(vec!{1, 2, 3, 4, 5}, 11), 40);
    }

    #[test]
    fn calc_pt2_aoc_example() {
        assert_eq!(calc_pt1(vec!{1721, 979, 366, 299, 675, 1456}, 2020), 241861950);
    }
}