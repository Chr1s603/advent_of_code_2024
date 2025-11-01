export module aoc.days;

import std;

import util.types;

import aoc.runner;
import aoc.day01;
import aoc.day02;
import aoc.day03;
import aoc.day04;
import aoc.day05;
import aoc.day06;
import aoc.day07;
import aoc.day08;
import aoc.day09;
import aoc.day10;
import aoc.day11;
import aoc.day12;
import aoc.day13;
import aoc.day14;
import aoc.day15;
import aoc.day16;
import aoc.day17;
import aoc.day18;
import aoc.day19;
import aoc.day20;
import aoc.day21;
import aoc.day22;
import aoc.day23;
import aoc.day24;

export namespace aoc {
inline constexpr arr<runner, 24> registry
    = {make_runner<day01::Day01>(), make_runner<day02::Day02>(), make_runner<day03::Day03>(),
       make_runner<day04::Day04>(), make_runner<day05::Day05>(), make_runner<day06::Day06>(),
       make_runner<day07::Day07>(), make_runner<day08::Day08>(), make_runner<day09::Day09>(),
       make_runner<day10::Day10>(), make_runner<day11::Day11>(), make_runner<day12::Day12>(),
       make_runner<day13::Day13>(), make_runner<day14::Day14>(), make_runner<day15::Day15>(),
       make_runner<day16::Day16>(), make_runner<day17::Day17>(), make_runner<day18::Day18>(),
       make_runner<day19::Day19>(), make_runner<day20::Day20>(), make_runner<day21::Day21>(),
       make_runner<day22::Day22>(), make_runner<day23::Day23>(), make_runner<day24::Day24>()};
}
