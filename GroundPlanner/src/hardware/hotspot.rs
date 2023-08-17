use std::thread::{JoinHandle};

pub struct Hotspot{
    devices: Vec<Device>,
    ssid: String,
    hotspot: JoinHandle<()>,
}
struct Device{
    ip: [u8; 4],
}

impl Hotspot{
    fn start() -> Self{
        
    }
}
