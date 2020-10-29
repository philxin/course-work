
app.df <- read.csv("apps.csv")
library(ggplot2)
# bar chart of category
catg.barplot <-ggplot(data=app.df, aes(x=Category)) +
  geom_bar(stat="count", fill="steelblue") + ggtitle("Bar chart of categories") +
  theme(axis.text.x=element_text(angle=50, hjust = 1)); catg.barplot

# Histogram of rating
hist.rating <- ggplot(data=app.df, aes(x=Rating)) +
  geom_histogram(stat="bin", fill="steelblue") + 
  ggtitle("Histogram of Rating"); hist.rating

# boxplot of rating vs category
boxp.rating.catg <- ggplot(data=app.df, aes(x=Category,y=Rating)) +
  geom_boxplot() + ggtitle("Box Plot of Rating v.s. Category") +
  theme(axis.text.x=element_text(angle=50, hjust = 1)); boxp.rating.catg


# histogram of price
hist.price <- ggplot(data=app.df, aes(x=Price)) +
  geom_area(stat="bin", fill="steelblue") + 
  ggtitle("Histogram of Price"); hist.price

# scatterplot of rating vs price
scat.rating.price <- ggplot(data=app.df, aes(x=Price,y=Rating)) +
  geom_point() + ggtitle("Scatter Plot of Rating v.s. Price") ; scat.rating.price 

# histogram of reviews
# scatter plot of rating vs reviews
scat.rating.review <- ggplot(data=app.df, aes(x=Reviews,y=Rating)) +
  geom_point() + ggtitle("Scatter Plot of Rating v.s. Reviews") ; scat.rating.review 


# scatter plot of rating vs installs
scat.rating.install <- ggplot(data=app.df, aes(x=Installs,y=Rating)) +
  geom_point() + ggtitle("Scatter Plot of Rating v.s. Installs") + 
  theme(axis.text.x=element_text(angle=50, hjust = 1)); scat.rating.install


# histogram of size
# scatter plot of rating vs size
scat.rating.size <- ggplot(data=app.df, aes(x=Size,y=Rating)) +
  geom_point() + ggtitle("Scatter Plot of Rating v.s. Size"); scat.rating.size

# bar chart of content rating
barplot.content <-ggplot(data=app.df, aes(x=Content.Rating)) +

  geom_bar(stat="count", fill="steelblue") + ggtitle("Bar chart of Content Rating") +
  theme(axis.text.x=element_text(angle=50, hjust = 1)); barplot.content


# boxplot of rating vs content rating
boxp.rating.content <- ggplot(data=app.df, aes(x=Content.Rating,y=Rating)) +
  geom_boxplot() + ggtitle("Box Plot of Rating v.s. Content.Rating") +
  theme(axis.text.x=element_text(angle=50, hjust = 1)); boxp.rating.content


# histogram of days from last update (< 500 days)
hist.lastUpdate <- ggplot(data=app.df, aes(x=DaysFromLastUpdate)) +
  geom_histogram(stat="count", fill="steelblue") + 
  ggtitle("Histogram of Rating") + 
  scale_x_continuous(limits = c(0,500)); hist.lastUpdate


# scatter plot of rating vs days from last update (< 500 days)
scat.rating.size <- ggplot(data=app.df, aes(x=DaysFromLastUpdate,y=Rating)) +
  geom_point() + ggtitle("Scatter Plot of Rating v.s. Days From Last Update") + 
  scale_x_continuous(limits = c(0,500)); scat.rating.size

###################### regression 
#typeof(app.df$Reviews)
#class(app.df$Reviews)
#app.df$Reviews <- as.numeric(as.character(app.df$Reviews))

require(Sleuth3)
require(mosaic)
lm1 = lm(Rating ~ log(Reviews) + Size + log(Installs) + Price + 
            Genres + DaysFromLastUpdate, data=app.df)
summary(lm1)
plot(lm1, which=1)



# Define the smallest model
smallest <- Rating~ 1
# Define the biggest model
biggest <- Rating ~ Category + log(Reviews) + Size + log(Installs) + Price + 
  Content.Rating + Genres + DaysFromLastUpdate
# define the model to start
m <- lm(Rating ~ 1, data=app.df)
# it does forward selection of the variables for us
stats::step(m, scope=list(lower=smallest, upper=biggest)) 

# result of forward selection
lm2 = lm(formula = Rating ~ log(Reviews) + log(Installs) + Category + 
           DaysFromLastUpdate + Size + Price, data = app.df)
summary(lm2)
plot(lm2, which=1)

confint(lm2)[3,]

pred2 = predict(lm2, se.fit = TRUE, newdata = app.df)
pred.fit <- pred2$fit


# ch12 Q14 (c)
# Define the biggest model
biggest <- Rating ~ Category + log(Reviews) + Size + log(Installs) + Price + 
  Content.Rating + Genres + DaysFromLastUpdate
m <- lm(biggest, data=app.df)
# it does backward selection of the variables for us
stats::step(m, direction="backward") 

# ch12 Q14 (d)
# Define the smallest model
smallest <- Rating~ 1
# Define the biggest model
biggest <- Rating ~ Category + log(Reviews) + Size + log(Installs) + Price + 
  Content.Rating + Genres + DaysFromLastUpdate
# define the model to start
m <- lm(Rating~ 1, data=app.df)
# it does stepwise selection of the variables for us
stats::step(m, scope=list(lower=smallest, upper=biggest), direction="both")
