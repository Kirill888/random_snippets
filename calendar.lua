color_table = {
  today   = "white"
, weekday = "orange"
, weekend = "F08080"
, other   = "gray"
}

function format_today(d)
   return string.format("${color %s}%2d${color}", color_table["today"], d)
end

function format_weekday(d)
   return string.format("${color %s}%2d${color}", color_table["weekday"], d)
end

function format_weekend(d)
   return string.format("${color %s}%2d${color}", color_table["weekend"], d)
end

function format_othermonth(d)
   return string.format("${color %s}%2d${color}", color_table["other"], d)
end


function formatDay(d, today)
   if d["month"] == today["month"] then
      if d["day"] == today["day"] then
	 return format_today(d["day"])
      end

      if d["wday"] == 1 or d["wday"] == 7 then
	 return format_weekend(d["day"])
      end

      return format_weekday(d["day"])
   end

   return format_othermonth(d["day"])
end

function formatWeek(t, today )
   local ONE_DAY = 24*60*60

   local s = formatDay(os.date("*t",t), today)

   for i = 1,6 do
      t = t + ONE_DAY
      s = s .. " " .. formatDay(os.date("*t",t), today)
   end
  
   return s,(t+ONE_DAY)
end

function conky_calendar()
  local ONE_DAY = 24*60*60
  local t = os.time()
  local today = os.date("*t",t)

  local adjust_ = {6,0,1,2,3,4,5}
  --local adjust_ = {0,1,2,3,4,5,6}

  local t1 = os.time({year  =today["year"]
                      ,month=today["month"]
                      ,day=1,hour=1,min=0,sec=0 })

  -- First day of the month
  local d1 = os.date("*t", t1)
  -- First day of the first week of the month
  local t0 = t1 - adjust_[ d1['wday'] ]*ONE_DAY

  local ss,t,s

  ss,t = formatWeek(t0,today)

  s,t  = formatWeek(t,today)
  ss   = ss .. "\n" .. s

  s,t = formatWeek(t,today)
  ss = ss .. "\n" .. s

  s,t = formatWeek(t,today)
  ss = ss .. "\n" .. s

  s,t = formatWeek(t,today)
  ss = ss .. "\n" .. s

  s,t = formatWeek(t,today)
  ss = ss .. "\n" .. s

  return ss
end

do
  ss = conky_calendar()
  print(ss)
end