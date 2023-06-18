use crate::score::block::Vector2i;
use rand::Rng;
use std::cell::{RefCell};

pub trait RandomGenerator {
    fn generate(&self, max: &Vector2i) -> Vector2i;
}

pub struct DefaultGenerator {
    rng: RefCell<rand::rngs::ThreadRng>,
}

pub struct FunctionalPointerGenerator {
    refer_to: RefCell<fn(max: &Vector2i) -> Vector2i>
}

impl FunctionalPointerGenerator {
    pub fn new(ptr: fn(max: &Vector2i) -> Vector2i) -> Self {
        Self { 
            refer_to: RefCell::new(ptr)
        }
    }
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

impl RandomGenerator for FunctionalPointerGenerator {
    fn generate(&self, max: &Vector2i) -> Vector2i {
        self.refer_to.borrow_mut()(max)
    }
}
