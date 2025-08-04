#include "main_menu.hpp"

void MainMenu::Start()
{

}

void MainMenu::Update()
{
  NQME::UpdateInputs();
  if(NQME::Pressed(KEY_A))
  {
    printf("A pressed\n");
    sm->SwitchTo(1);
  }
}

void MainMenu::Cleanup()
{
}
