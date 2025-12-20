# FoodRater

FoodRater is a C program that evaluates food products based on
energy content, nutritional quality and price, generating a
convenience ranking.

The goal is to help compare foods in an objective way using
simple nutritional data per 100 grams.

---

## Features

- Add, remove, search and edit food products
- Automatic calculation of:
  - Energy score
  - Nutritional value score
  - Final convenience score
- Ranking of products based on final score
- Persistent storage using file I/O
- ASCII banner and interactive menu

---

## Evaluation Method

Each product is evaluated using nutritional data per 100 grams.

### Energy Score
Energy is calculated from macronutrients:

Energy = (4 × proteins) + (9 × fats) + (4 × carbohydrates)

### Nutritional Value Score
Nutritional quality favors protein content over fats and carbohydrates:

Quality = proteins / (proteins + fats + carbohydrates)

Nutritional Value = Energy × Quality

### Final Score
The final convenience score is calculated as:

Final Score = Nutritional Value / Price

A higher final score indicates better value for money.

## Build and Run
```bash
gcc main.c -o FoodRater
./FoodRater
```



## Author

ShiroiShi  
Computer Science student 


