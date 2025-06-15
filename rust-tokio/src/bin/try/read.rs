use tokio::fs::File; // 1.45.0
use tokio::io::{self,AsyncReadExt};

#[tokio::main]
async fn main() ->io::Result<()> {
    let mut f = File::open("foo.txt").await?;
    let mut buffer = [0;10];
    let n = f.read(&mut buffer[..]).await?;
    println!("the bytes: {:?}",&buffer[..n]);
    Ok(())
}