use my_redis::connection::Connection;
use tokio::net::TcpStream;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // 连接到服务器
    let stream = TcpStream::connect("127.0.0.1:6379").await?;
    
    // 创建 Connection 实例
    let connection = Connection::new(stream);
    
    println!("成功创建连接！");
    
    // 在这里添加更多代码来使用 connection...
    
    Ok(())
}
