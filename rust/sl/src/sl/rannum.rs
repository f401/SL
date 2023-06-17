use rand::Rng;
use std::{cell::RefCell};
use crate::sl::block::Vector2i;

pub trait RandomGenerator {
    fn generate(&self, max: &Vector2i) -> Vector2i;
}

pub struct DefaultGenerator {
    rng: RefCell<rand::rngs::ThreadRng>,
}

impl DefaultGenerator {
    pub fn new() -> Self {
        Self {
            rng: RefCell::new(rand::thread_rng()),
        }
    }
}

impl RandomGenerator for DefaultGenerator {
    fn generate(&self, max: &Vector2i) -> Vector2i {
        let mut rng = self.rng.borrow_mut();
        return Vector2i::new(rng.gen_range(0..max.x), rng.gen_range(0..max.y));
    }
}
