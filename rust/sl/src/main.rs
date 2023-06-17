mod sl;

use sl::block::Vector2i;

fn main() {
    let vec = sl::game::Game::new(Vector2i::new(10, 20));
    vec.print();

}
