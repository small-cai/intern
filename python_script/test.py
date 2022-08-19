import pandas as pd
import calendar
import datetime
import pendulum

data = pd.read_csv(r"C:/Users/93567/Desktop/bike_sharing_dc.csv")

xing = []  # 星期几
apm = []  # 上下午
work = []  # 工作与否
print(data)
ld = {"Monday":1,"Tuesday":2,"Wednesday":3,"Thursday":4,"Friday":5,"Saturday":6,"Sunday":7}
for index, row in data.iterrows():
    Time = row["date"]
    Hour = row["hour"]
    if int(Hour) <= 12:
        apm.append("am")
    elif (int(Hour) > 12):
        apm.append("pm")
    Year = int(row["year"])
    Month = int(row["month"])
    Time_l = Time.split("/")
    # Time_l = str(Time.replace("/","-"))
    # print(Time_l)
    day = int(Time_l[-1])
    # print(Year, Month, day)
    # print(day)
    # print(Time_l)
    # print(Time)
    # Weekday = pendulum.parse(Time_l).day_of_week
    # print(Weekday)
    date_week = datetime.date(Year, Month, day).weekday()
    cal = calendar.day_name[date_week]
    # print(ld[cal])
    xing.append(ld[cal])
    # if Weekday == 0:
    #     Weekday = 7
    Weekday = ld[cal]
    if 6 <= Weekday <= 7:
        work.append(0)
    else:
        work.append(1)
data["work yes or not"] = work
data["am or pm"] = apm
data["Day of the week"] = xing
data.to_csv(r"C:/Users/93567/Desktop/bike_sharing_dc.csv", index=False)
print("YES")
# print(items[1])
