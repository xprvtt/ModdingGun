double calculateSuccessChance(double baseChance, double toolFactor, double kitFactor, double skillFactor)
{
    return baseChance * (1 + skillFactor) * (1 + toolFactor) * (1 + kitFactor);
}