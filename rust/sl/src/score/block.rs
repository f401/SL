use super::game::{Game, self};

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
    pub x: i32,
    pub y: i32,
}

#[derive(Debug)]
pub struct Block {
    pub position: Vector2i,
    pub status: Status,
    pub what: What,
}

impl Vector2i {
    pub fn new(x: i32, y: i32) -> Self {
        Self { x, y }
    }

    pub fn right(&self, game: &Game) -> Option<Self> {
        if self.x == game.max.x - 1 {
            return None;
        }
        return Some(Vector2i::new(self.x + 1, self.y));
    }

    pub fn left(&self) -> Option<Self> {
        if self.x == 0 {
            return None;
        }
        return Some(Vector2i::new(self.x - 1, self.y));
    }

    pub fn up(&self, game: &Game) -> Option<Self> {
        if self.y == game.max.y - 1 {
            return None;
        }
        return Some(Vector2i::new(self.x, self.y + 1));
    }

    pub fn down(&self) -> Option<Self> {
        if self.y == 0 {
            return None;
        }
        return Some(Vector2i::new(self.x, self.y - 1));
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
