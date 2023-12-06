#ifndef _SLEEP_TIME_H
#define _SLEEP_TIME_H

class Sleep_Time {
  public:
    // NTP server to request epoch time
    const char* ntpServer = "pool.ntp.org";

  // Constructor, must exists
    Sleep_Time(){
    }
    void init(){
      configTime(0, 0, ntpServer);
    }

    // Function that gets current epoch time
    time_t getTime() {
      time_t now;
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        //Serial.println("Failed to obtain time");
        return(0);
      }
      time(&now);
      return now;
    }

    // Function to get the epoch time of the 10th of the next month at the specified day and hour
    time_t getNextMonthTime(time_t currentTime, int targetDay, int targetHour) {
      // Get the current time information
      struct tm *currentTimeInfo = localtime(&currentTime);
      
      // Calculate the next month
      int nextMonth;
      int nextYear;
      
      // If the date is < 10 in any month, or if it's on the 10th of any month and current time is before mid-day, nextMonth stays the same.
      if ((currentTimeInfo->tm_mday < 10) || (currentTimeInfo->tm_mday == 10)&&(currentTimeInfo->tm_hour < 12)){
        nextMonth = currentTimeInfo->tm_mon;
      }else{ // else increment nextMonth by 1
        nextMonth = (currentTimeInfo->tm_mon)%12 + 1;
      }
      // If nextMonth==1, then we know for sure nextYear would get increamented by 1, // else nextYear stays the same
      nextYear = (nextMonth == 1) ? currentTimeInfo->tm_year + 1 : currentTimeInfo->tm_year;
      // if (nextMonth == 1){
      //   nextYear = currentTimeInfo->tm_year + 1;
      // }else{ 
      //   nextYear = currentTimeInfo->tm_year;
      // }
      

      // Set the time to 12:00 (noon)
      struct tm targetTimeInfo;
      targetTimeInfo.tm_year = nextYear;
      targetTimeInfo.tm_mon = nextMonth;
      targetTimeInfo.tm_mday = targetDay;
      targetTimeInfo.tm_hour = targetHour;
      targetTimeInfo.tm_min = 0;
      targetTimeInfo.tm_sec = 0;

      // Convert the struct to epoch time
      time_t targetTime = mktime(&targetTimeInfo);
      
      return targetTime;
    }

    String convertEpochToStandardTime(time_t epochTime){
      // Convert epoch time to standard time
      struct tm *tmTime = localtime(&epochTime);
      String converted_time;
      
      // Extract the individual components of the standard time
      int year = tmTime->tm_year + 1900;   // Year (e.g., 2023)
      int month = tmTime->tm_mon + 1;      // Month (1-12)
      int day = tmTime->tm_mday;           // Day of the month (1-31)
      int hour = tmTime->tm_hour;          // Hour (0-23)
      int minute = tmTime->tm_min;         // Minute (0-59)
      int second = tmTime->tm_sec;         // Second (0-59)

      // Concatenate the standard time components
      converted_time = String(day)+"/"+String(month)+"/"+String(year)+"  "+String(hour)+":"+String(minute)+":"+String(second);
      
      return converted_time;
    }

    String convertEpochDiffToStandardTime(time_t epochDiff){
      // Convert the epoch time difference to seconds, minutes, hours, days, months, and years
      int seconds = epochDiff % 60;
      int minutes = (epochDiff / 60) % 60;
      int hours = (epochDiff / 3600) % 24;
      int days = (epochDiff / (24 * 3600)) % 30;
      int months = (epochDiff / (30 * 24 * 3600)) % 12;
      int years = epochDiff / (365 * 24 * 3600);

      String Seconds = (seconds==1) ? " second" : " seconds";
      String Minutes = (minutes==1) ? " minute" : " minutes";
      String Hours = (hours==1) ? " hour" : " hours";
      String Days = (days==1) ? " day" : " days";
      String Months = (months==1) ? " month" : " months";
      String Years = (years==1) ? " year" : " years";

      String time_diff = String(seconds)+Seconds+", "+String(minutes)+Minutes+", "+String(hours)+Hours+", "+String(days)+Days+", "+String(months)+Months+", "+String(years)+Years;

      return time_diff;
    }
};

#endif // _SLEEP_TIME_H

