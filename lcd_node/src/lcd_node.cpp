//2022 Copyright (c) Mario Mata.  All right reserved. MIT License
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "lcd_i2c.h"
#include <iostream>
class i2cLCD
{
  public:
    i2cLCD();

  private:
    void stringCallback(const std_msgs::String::ConstPtr& msg);
    ros::NodeHandle nh;
    ros::Subscriber str_sub;
    LCD_I2C *lcd;
    int colorR, colorG, colorB;
    char IP_addr[17];
};
//constructor
i2cLCD::i2cLCD()
{
  str_sub = nh.subscribe<std_msgs::String>("/LCDstring", 10, &i2cLCD::stringCallback, this);
  lcd = NULL;
  //other initializations here
  colorR=0;
  colorG=255;
  colorB=255;
    //Get IP address from ifconfig wlan0
    FILE *fp;
    char aux[100];
    const char *p_aux;
    fp=popen("ifconfig wlan0","r");
    if(fp!=NULL) //check if we have IP
    { 
      fgets(aux,100,fp); //get first line -discarded
      fgets(aux,100,fp); //get second line -contains inet or inet6-
      //find 'i' from "inet" in the extracted string
      p_aux = strchr(aux,'i');
      sscanf(p_aux+10,"%17s",IP_addr);
      fclose(fp);
    }
}
//string topic callback. Do things here
void i2cLCD::stringCallback(const std_msgs::String::ConstPtr& msg)
{
 size_t n=0;
 //printf("Received: %s\n", msg->data.c_str());
 std::string a = msg->data;
 //Interpret received string
 if(a[0]=='$') //This is a command
 {
  n=a.find("Color:"); //Must be '$Color:rrr,ggg,bbb'
  if(n==1)//color command found, get color components
  {
    if(a.size()<13) //not long enough to contain RGB color
     return;
    std::string b=a.substr(7,16); //skip '$Color:'
    sscanf(b.c_str(),"%d,%d,%d",&colorR,&colorG,&colorB); //read R component
    //n=b.find(',');
    ROS_INFO("New LCd Color: %d, %d, %d\n",colorR,colorG,colorB); 
  }
 }
 else //normal string, print it
 {
  // Print at most 16 characters, always in LCD row 2
  std::string b=a.substr(0,16); //limit lenght
  //Connect to LCD, update and disconnect again
    lcd = new LCD_I2C(16,2); //cols, rows
    if (lcd->get_error() <0) 
    {
     ROS_INFO("LCD Error: %d\n", lcd->get_error());
    }
    else
    {
     lcd->setRGB(colorR, colorG, colorB);
     lcd->setCursor(0,0);    
     lcd->prints(IP_addr); //output IP status to the LCD
     lcd->setCursor(0, 1);
     lcd->prints(b.c_str());
     delete(lcd);
    }
 }
}
//launch node
int main(int argc, char **argv)
{
  ros::init(argc, argv, "lcd_node");
  i2cLCD i2c_lcd;
  ros::spin();
}
