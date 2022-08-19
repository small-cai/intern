import datetime
import calendar
 
date_week = datetime.date(2011, 1, 1).weekday()
cal = calendar.day_name[date_week]
print(cal)