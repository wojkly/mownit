
results = read.csv("data2.txt")

results$speed = results$n / results$time

avg_results = aggregate( cbind(time,speed) ~ n:alg, data=results, FUN=mean) 

ggplot(avg_results,aes(n,time,color=alg)) + geom_point() + ylab("Computing time in seconds")

ggsave("myplot.png")