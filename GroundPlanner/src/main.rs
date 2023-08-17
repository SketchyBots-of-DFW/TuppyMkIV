use iced::widget::{button, column, text};
use iced::{Alignment, Element, Sandbox, Settings};

fn main() -> iced::Result {
    Planner::run(Settings::default())
}

struct Planner{
    connect_status: bool,
}

impl Sandbox for Planner{
    type Message = bool;
    fn new() -> Self{
        Self {connect_status: false}
    }
    fn title(&self) -> String{
        String::from("Ground Planner")
    }
    fn update(&mut self, connect_status: bool){
        match connect_status{
            true => {
                self.connect_status = true;
            }
            false => {
                self.connect_status = false;
            }
        }
    }
    fn view(&self) -> Element<bool>{
        column![
            button("Make true").on_press(true),
            text(self.connect_status).size(50),
            button("Make false").on_press(false),
        ]
            .padding(20)
            .align_items(Alignment::Center)
            .into()
    }
}
