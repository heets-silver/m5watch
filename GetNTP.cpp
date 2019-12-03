#include <WiFiUdp.h>
#include <NTPClient.h>
#include "M5_Watch.h"

#define NTP_OFFSET  3600 * 9 // Seoul
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "kr.pool.ntp.org"
  
Ymdhms getntp()
{
  String formattedDate;
  Ymdhms ymdhms;
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET);

  timeClient.begin();
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();
  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  
  String dateStamp = formattedDate.substring(0, splitT);
  String dayStamp = formattedDate.substring(splitT-2, splitT);
  String monthStamp = formattedDate.substring(splitT-5, splitT-3);
  String yearStamp = formattedDate.substring(0, 4);
  String yearStampSmall = formattedDate.substring(2, 4);
  String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  String hourStamp = timeStamp.substring(0,2);
  String minuteStamp = timeStamp.substring(3,5);
  String secondStamp = timeStamp.substring(6,8);

  int year = yearStamp.toInt();
  int month = monthStamp.toInt();
  int day = dayStamp.toInt();
  int hour = hourStamp.toInt();
  int minute = minuteStamp.toInt();
  int second = secondStamp.toInt();

  boolean isPm = false;
  
  int hourNoMilitary;
  
  if(hour > 12)
  {
    hourNoMilitary = hour - 12;
    isPm = true;
  }
  else
  {
    isPm = false;
    hourNoMilitary = hour;
  }
  if(hour == 0)
  {
    hour = 0;
    hourNoMilitary = 12;
  }
  String hourStampNoMilitary;
  if(hourNoMilitary < 10)
  {
    hourStampNoMilitary = "0"+(String)hourNoMilitary;
  }
  else
  {
    hourStampNoMilitary = (String)hourNoMilitary;
  }

  int daysLeft = 0;
  int monthsLeft = 0;

  String timeStampNoMilitary = hourStampNoMilitary + ":" + minuteStamp + ":" + secondStamp;    //hh:mm:ss
  String dateStampConstructed = monthStamp + "/" + dayStamp + "/" + yearStampSmall;  //mm/dd/yy
  ymdhms.year = year;
  ymdhms.mon  = month;
  ymdhms.day  = day;
  ymdhms.hh   = hour;
  ymdhms.mm   = minute;
  ymdhms.ss   = second;
  return ymdhms;
}
