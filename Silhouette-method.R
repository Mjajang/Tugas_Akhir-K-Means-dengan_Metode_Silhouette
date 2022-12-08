#load library
library(factoextra)
library(cluster)
#library(NbClust)

#load datasensor
datasensor <- read.csv(file.choose())
head(datasensor)

#pilih kolom yang akan dihitung/buat dataframe
df <- datasensor[,2:6]
head(df)

#scaling dataframe
df <- scale(df)
head(df)


fviz_nbclust(df, kmeans, method = "silhouette") +
  labs(subtitle = "Silhouette method")



