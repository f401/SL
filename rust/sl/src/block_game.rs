use std::{alloc, ptr, usize};
use std::{alloc::Layout, ptr::NonNull};

#[derive(Debug)]
pub enum Status {
    Open,
    Close,
    Flag,
}

#[derive(Debug)]
pub enum What {
    Num(usize),
    Empty,
    Mine,
}

#[derive(Debug)]
pub struct Vector2i {
    x: usize,
    y: usize,
}

impl Vector2i {
    pub fn new(x: usize, y: usize) -> Self {
        Self { x, y }
    }
}

#[derive(Debug)]
pub struct Block {
    position: Vector2i,
    status: Status,
    what: What,
}

impl Block {
    pub fn new(position: Vector2i, status: Status, what: What) -> Self {
        Self {
            position,
            status,
            what,
        }
    }
}

pub struct Game {
    max: Vector2i,
    ptr: NonNull<Block>,
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

    pub fn at(&self, pos: Vector2i) -> Block {
        unsafe { ptr::read(self.ptr.as_ptr().add(pos.x + pos.y * self.max.x)) }
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
                    self.at(Vector2i::new(current_x, current_y))
                );
                current_y += 1;
            }
            current_x += 1;
        }
    }
}
