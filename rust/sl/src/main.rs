mod score;

use score::block::Vector2i;

fn main() {
    let mut vec = score::game::Game::new(Vector2i::new(10, 20));
    vec.print();
    println!("After Generate");
    vec.generate_mines_default(20);
    vec.print_layout();
    vec.generate_numbers();
    println!();
    vec.print_layout();
}
