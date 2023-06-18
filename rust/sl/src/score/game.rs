use super::block::{Block, Status, Vector2i, What};
use super::rannum::RandomGenerator;
use std::cell::Ref;
use std::ops::{Index, IndexMut};
use std::{alloc, ptr, usize};
use std::{alloc::Layout, ptr::NonNull};

use super::rannum::DefaultGenerator;

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
                    ptr.add(current_x + current_y * max.x),
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

    fn index(& self, index: Vector2i) -> &Self::Output {
        unsafe { & *(self.ptr.as_ptr().add(index.x + index.y * self.max.x)) }
    }
}

impl IndexMut<Vector2i> for Game {
    fn index_mut(&mut self, index: Vector2i) -> &mut Self::Output {
        unsafe { &mut *(self.ptr.as_ptr().add(index.x + index.y * self.max.x)) }
    }
}

impl Game {
    pub fn new(max: Vector2i) -> Self {
        let layout = Layout::array::<Block>(max.x * max.y).expect("Layout Failed!");
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
}

fn test90() {
    let mut vec = vec![1, 2, 3];
    vec.index(123);
}
