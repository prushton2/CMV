mod process_attacher;

// use crate::process_attacher;

fn main() {
    let r = process_attacher::exec("./a.out");
    println!("{:?}", r);
}