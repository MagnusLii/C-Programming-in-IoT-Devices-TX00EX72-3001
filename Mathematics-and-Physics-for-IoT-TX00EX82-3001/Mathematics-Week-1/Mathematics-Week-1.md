
# 1
$f(t) = cos(t+2), t ∈ R$ <br>
$f(0) = 1$ <br>
$f(1) = -0.9899$ <br>
$f(2) = -0.6536$ <br>
$f(3) = 0.28366$ <br>
$f(4) = 0.96017$ <br>
$f(5) = 0.75390$ <br>

# 2
$x(k)= k^2/2 + k$ <br>
$x(0) = 0$ <br>
$x(1) = 3/2$  
$x(2) = 4$  
$x(3) = 15/2$  
$x(4) = 12$  

# 3
library(ggplot2)
library(dplyr)
library(glue)

iris_cor <- iris %>% 
  group_by(Species) %>%
  summarize(r_square = cor(Sepal.Length, Sepal.Width)^2) %>%
  mutate(
    # location of each text label in data coordinates
    Sepal.Length = 8, Sepal.Width = 4.5,
    # text label containing r^2 value 
    label = glue("r^2 = {round(r_square, 2)}")
  )

iris_cor
#> # A tibble: 3 × 5
#>   Species    r_square Sepal.Length Sepal.Width label     
#>   <fct>         <dbl>        <dbl>       <dbl> <glue>    
#> 1 setosa        0.551            8         4.5 r^2 = 0.55
#> 2 versicolor    0.277            8         4.5 r^2 = 0.28
#> 3 virginica     0.209            8         4.5 r^2 = 0.21

iris_facets <- ggplot(iris, aes(Sepal.Length, Sepal.Width)) +
  geom_point() + 
  geom_smooth(method = "lm", formula = y ~ x) +
  facet_wrap(~Species) +
  theme_bw()

iris_facets + 
  geom_text(
    data = iris_cor,
    aes(label = label),
    hjust = 1, vjust = 1
  )