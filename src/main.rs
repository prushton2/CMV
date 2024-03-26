use gdb_mi;

fn main() {
    println!("init session");
    let mut session = match gdb_mi::Session::new("gdb", &["./main".to_string()]) {
        Ok(c) => c,
        Err(_c) => return,
    };
    println!("making breakpoint");

    //set bp
    let bp_create = match session.send("b 1\n") {
        Ok(c) => c,
        Err(c) => {println!("Err: {:?}", c); return;}
    };


    println!("Running");
    let run = match session.send("r\n") {
        Ok(c) => c,
        Err(c) => {println!("Err: {:?}", c); return;}
    };




    println!("receive breakpoint");

    let bp_create_rec = match session.receive() {
        Ok(c) => c,
        Err(c) => {println!("Err: {:?}", c); return;}
    };

    println!("{:?}", bp_create_rec);

    println!("aa");
}
