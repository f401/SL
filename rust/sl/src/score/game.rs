use super::around::get_around_with;
use super::block::{Block, Status, Vector2i, What};
use super::rannum::RandomGenerator;
use std::cell::{Ref, RefCell};
use std::ops::{Index, IndexMut};
use std::rc::Rc;
use std::{alloc, ptr, usize};
use std::{alloc::Layout, ptr::NonNull};

use super::rannum::DefaultGenerator;

use colored::*;

pub struct Game {
    pub max: Vector2i,
    pub ptr: NonNull<Block>,
}

fn init_block_ptr(max: &Vector2i, ptr: *mut Block) -> *mut Block {
    let mut current_x = 0;
    while current_x < max.x {
        let mut current_y = 0;
        while current_y < max.y {
            unsafe {
                ptr::write(
                    ptr.add((current_x + current_y * max.x) as usize),
                    Block::new(
                        Vector2i::new(current_x, current_y),
                        Status::Close,
                        What::Empty,
                    ),
                );
            }
            current_y += 1;
        }
        current_x += 1;
    }
    ptr
}

impl Index<Vector2i> for Game {
    type Output = Block;

    fn index(&self, index: Vector2i) -> &Self::Output {
        unsafe { &*(self.ptr.as_ptr().add((index.x + index.y * self.max.x) as usize)) }
    }
}

impl IndexMut<Vector2i> for Game {
    fn index_mut(&mut self, index: Vector2i) -> &mut Self::Output {
        unsafe { &mut *(self.ptr.as_ptr().add((index.x + index.y * self.max.x) as usize)) }
    }
}

impl Drop for Game {
    fn drop(&mut self) {
        unsafe {
            alloc::dealloc(
                self.ptr.as_ptr() as *mut u8,
                Layout::array::<Block>((self.max.x * self.max.y) as usize).expect("Error When Drop"),
            );
        }
    }
}

impl Game {
    pub fn new(max: Vector2i) -> Self {
        let layout = Layout::array::<Block>((max.x * max.y) as usize).expect("Layout Failed!");
        let ptr = unsafe { alloc::alloc(layout) };
        let block_ptr = init_block_ptr(&max, ptr as *mut Block);
        Self {
            max,
            ptr: NonNull::new(block_ptr).expect("error when create NonNUll"),
        }
    }

    pub fn generate_mines_default(&mut self, count: usize) {
        self.generate_mines(count, DefaultGenerator::new());
    }

    pub fn generate_mines(&mut self, count: usize, generator: impl RandomGenerator) {
        let mut current = 0;
        while current < count {
            let mut refer_to = self.index_mut(generator.generate(&self.max));
            if let What::Empty = refer_to.what {
                refer_to.what = What::Mine;
                current += 1;
            } else if let What::Num(_) = refer_to.what {
                panic!("Mines Been Generated Finished");
            }
        }
    }

    pub fn generate_numbers(&mut self) {
        let mut current_x = 0;
        while current_x < self.max.x {
            let mut current_y = 0;
            while current_y < self.max.y {
                let current = self.index(Vector2i::new(current_x, current_y));
                if let What::Mine = current.what {
                    let around = get_around_with(&current.position, &self);
                    for i in around.into_iter() {
                        let curr_blk = self.index_mut(i);
                        match curr_blk.what {
                            What::Mine => {} 
                            What::Num(ref mut current_num) => *current_num += 1,
                            What::Empty => curr_blk.what = What::Num(1),
                        }
                    }
                }
                current_y += 1;
            }
            current_x += 1;
        }
    }

    pub fn print(&self) {
        let mut current_x = 0;
        while current_x < self.max.x {
            let mut current_y = 0;
            while current_y < self.max.y {
                println!(
                    "x: {}, y: {}: {:?}",
                    current_x,
                    current_y,
                    self.index(Vector2i::new(current_x, current_y))
                );
                current_y += 1;
            }
            current_x += 1;
        }
    }

    pub fn print_layout(&self) {
        let mut current_y = self.max.y - 1;
        while current_y >= 0 {
            print!("{} ", format!("{}", current_y).blue().underline());
            let mut current_x = 0;
            while current_x < self.max.x - 1{
                let curr = self.index(Vector2i::new(current_x, current_y));
                let sign = match curr.what {
                    What::Num(num) => num.to_string(),
                    What::Empty => String::from("E"),
                    What::Mine => String::from("M"),
                };
                print!("{} ", sign);
                current_x += 1;
            }
            println!("");
            current_y -= 1;
        }
    }
}
