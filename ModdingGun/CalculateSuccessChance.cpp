<<<<<<< HEAD
﻿double calculateSuccessChance(double baseChance, double toolFactor, double kitFactor, double skillFactor)
=======
﻿
double calculateSuccessChance(double baseChance, double toolFactor, double KitFactor, double skillFactor)
>>>>>>> main
{
    return baseChance * (1 + skillFactor) * (1 + toolFactor) * (1 + kitFactor);
}