## Data cleaning and processing

pp.df <- data.frame(stringsAsFactors = FALSE)
app.df <- read.csv("google-play-store-apps/googleplaystore.csv")
View(app.df)
# delete the row with problematic data
app.df <- app.df[-c(10473),]

library(stringr)
library(naniar)
# cleaning Size: get rid of "M" or "k", 
# replace "Varies with device" with mean of size in each category,
# and turn numeric

# first, replace "Varies with device" with N/A (and replace it with mean later)
app.df <- replace_with_na(app.df, replace=list(Size=c("Varies with device")))

# remove the unit (M or k), make this columns numeric and use M as the unit
size.temp <- as.character(app.df$Size)
for(i in 1:length(size.temp))
{
  if (is.na(size.temp[i]))
  {
    next
  }
  else if(str_sub(size.temp[i], -1, -1) == "M")
  {
    size.temp[i] <- str_sub(size.temp[i], end=-2)
  }
  else if (str_sub(size.temp[i], -1, -1) == "k")
  {
    size.temp[i] <- str_sub(size.temp[i], end=-2)
    size.temp[i] <- as.numeric(size.temp[i])/1024
  }
  else if (str_sub(size.temp[i], -1, -1) == "+")
  {
    size.temp[i] <- str_sub(size.temp[i], end=-2)
  }
}
size.temp <- as.numeric(size.temp)

df.temp <-app.df
df.temp$Size <-  size.temp

# calculate the mean size value of each category
size.meanByCatg <- aggregate(Size ~ Category, df.temp, FUN=mean)

# replace N/A (originally Varies with device) with the mean size value of each category
for(i in 1:nrow(df.temp))
{
  if (is.na(df.temp[i,]$Size))
  {
    df.temp[i,]$Size <- size.meanByCatg[size.meanByCatg$Category==df.temp[i,]$Category,]$Size
  }
}
app.df <- df.temp
app.df$Size <- round(app.df$Size, digits = 3)

View(app.df$Installs)
# cleaning installs: get rid of "+", and turn numeric
install.temp <- as.character(app.df$Installs)
for (i in 1:length(install.temp))
{
  if(str_sub(install.temp[i], -1, -1) == "+")
  {
    install.temp[i] <- str_sub(install.temp[i], end=-2)
  }
}
View(install.temp)
app.df$Installs <-install.temp
app.df$Installs <- as.numeric(gsub(",","",app.df$Installs))

# cleaning price: remove "$", and turn numeric
price.temp <- as.character(app.df$Price)
for (i in 1:length(price.temp))
{
  if(str_sub(price.temp[i], 1, 1) == "$")
  {
    price.temp[i] <- str_sub(price.temp[i], start=2, end=-1)
  }
}
app.df$Price <- as.numeric(price.temp)


# transform the Last.Updated column into date type
app.df$Last.Updated <- as.Date(app.df$Last.Updated, format="%B %d, %Y")
max(app.df$Last.Updated) # 2018-08-08 (the lastest updated date of all apps)

# add a column "DaysFromLastUpdate"
#(assume today is the day of the lastest updated date of all apps in the dataset (2018-08-08))
app.df$DaysFromLastUpdate <- as.Date("2018-08-08") - as.Date(app.df$Last.Updated, format="%B %d, %Y")

# turn Reviews into numeric
app.df$Reviews <- as.numeric(as.character(app.df$Reviews))

# output data frame to csv file
write.csv(app.df, "apps.csv")
