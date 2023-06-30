use super::game::Game;

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
    pub x: usize,
    pub y: usize,
}

#[derive(Debug)]
pub struct Block {
    pub position: Vector2i,
    pub status: Status,
    pub what: What,
}

impl Vector2i {
    pub fn new(x: usize, y: usize) -> Self {
        Self { x, y }
    }

    pub fn up(&self) -> Option<Self> {
        if self.x == 0 {
            return None;
        }
        return Some(Vector2i::new(self.x - 1, self.y));
    }
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
