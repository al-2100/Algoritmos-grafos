#Actualizar la ruta de rutakruskal y rutaprim para que funcione al descargar
#el proyecto
rutakruskal <- "C:/Users/Alonso/Desktop/Proyecto/cmake-build-debug/kruskal.txt"
rutaprim <- "C:/Users/Alonso/Desktop/Proyecto/cmake-build-debug/prim.txt"
datos_kruskal <- read.table(rutakruskal, header = FALSE)
datos_prim <- read.table(rutaprim, header = FALSE)
plot(datos_kruskal$V1, datos_kruskal$V2, main = "Kruskal", xlab = "Aristas", ylab = "Tiempo (s)")
plot(datos_prim$V1, datos_prim$V2, main = "Prim", xlab = "Aristas", ylab = "Tiempo (s)")
nrow(datos_prim)
nrow(datos_kruskal)
