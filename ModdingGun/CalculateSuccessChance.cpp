
double calculateSuccessChance(double baseChance, double toolFactor, double KitFactor, double skillFactor)
{
    return baseChance * (1 + skillFactor) * (1 + toolFactor) * (1 + KitFactor);
}