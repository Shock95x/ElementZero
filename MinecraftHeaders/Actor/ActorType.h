#pragma once

#include "../dll.h"
#include <functional>

enum class ActorType {
    Item = 0x40,
    TNT = 0x41,
    FallingBlock = 0x42,
    MovingBlock = 0x43,
    XpOrb = 0x45,
    EyeOfEnderSignal = 0x46,
    EnderCrystal = 0x47,
    FireworksRocket = 0x48,
    FishingHook = 0x4d,
    Chalkboard = 0x4e,
    Painting = 0x53,
    LeashKnot = 0x58,
    Boat = 0x5a,
    LightningBolt = 0x5d,
    AreaEffectCloud = 0x5f,
    Balloon = 0x6b,
    Shield = 0x75,
    Npc = 0x133,
    Agent = 0x138,
    ArmorStand = 0x13d,
    TripodCamera = 0x13e,
    Player = 0x13f,
    Bee = 0x17a,
    Piglin = 0x17b,
    PiglinBrute = 0x17f,
    IronGolem = 0x314,
    SnowGolem = 0x315,
    WanderingTrader = 0x376,
    Creeper = 0xb21,
    Slime = 0xb25,
    Enderman = 0xb26,
    Ghast = 0xb29,
    MagmaCube = 0xb2a,
    Blaze = 0xb2b,
    Witch = 0xb2d,
    Guardian = 0xb31,
    ElderGuardian = 0xb32,
    EnderDragon = 0xb35,
    Shulker = 0xb36,
    Vindicator = 0xb39,
    Ravager = 0xb3b,
    EvocationIllager = 0xb68,
    Vex = 0xb69,
    Pillager = 0xb72,
    ElderGuardianGhost = 0xb78,
    Chicken = 0x130a,
    Cow = 0x130b,
    Pig = 0x130c,
    Sheep = 0x130d,
    Mooshroom = 0x1310,
    Rabbit = 0x1312,
    PolarBear = 0x131c,
    Llama = 0x131d,
    Turtle = 0x134a,
    Panda = 0x1371,
    Fox = 0x1379,
    Hoglin = 0x137c,
    Strider = 0x137d,
    Goat = 0x1380,
    Axolotl = 0x1382,
    Squid = 0x2311,
    Dolphin = 0x231f,
    Pufferfish = 0x236c,
    Salmon = 0x236d,
    Tropicalfish = 0x236f,
    Cod = 0x2370,
    GlowSquid = 0x2381,
    Wolf = 0x530e,
    Ocelot = 0x5316,
    Parrot = 0x531e,
    Cat = 0x534b,
    Bat = 0x8113,
    ZombiePigman = 0x10b24,
    Wither = 0x10b34,
    Phantom = 0x10b3a,
    Zoglin = 0x10b7e,
    Zombie = 0x30b20,
    ZombieVillager = 0x30b2c,
    Husk = 0x30b2f,
    Drowned = 0x30b6e,
    ZombieVillagerV2 = 0x30b74,
    Spider = 0x40b23,
    Silverfish = 0x40b27,
    CaveSpider = 0x40b28,
    Endermite = 0x40b37,
    Minecart = 0x80054,
    MinecartHopper = 0x80060,
    MinecartTNT = 0x80061,
    MinecartChest = 0x80062,
    MinecartCommandBlock = 0x80064,
    Skeleton = 0x110b22,
    Stray = 0x110b2e,
    WitherSkeleton = 0x110b30,
    Horse = 0x205317,
    Donkey = 0x205318,
    Mule = 0x205319,
    SkeletonHorse = 0x215b1a,
    ZombieHorse = 0x215b1b,
    XpBottle = 0x400044,
    ShulkerBullet = 0x40004c,
    DragonFireball = 0x40004f,
    Snowball = 0x400051,
    Egg = 0x400052,
    Fireball = 0x400055,
    SplashPotion = 0x400056,
    EnderPearl = 0x400057,
    WitherSkull = 0x400059,
    WitherSkullDangerous = 0x40005b,
    SmallFireball = 0x40005e,
    LingeringPotion = 0x400065,
    LlamaSpit = 0x400066,
    EvocationFang = 0x400067,
    IceBomb = 0x40006a,
    ThrownTrident = 0xc00049,
    Arrow = 0xc00050,
    Villager = 0x100030f,
    VillagerV2 = 0x1000373
};

enum class ActorTypeNamespaceRules { ExcludeNamespace, IncludeNamespace };

MCAPI ActorType lookupActualEntityType(ActorType);
MCAPI void forEachEntityType(std::function<bool(ActorType, std::string const &)>);
MCAPI ActorType EntityTypeFromString(std::string const &);
MCAPI std::string EntityTypeToLocString(ActorType, ActorTypeNamespaceRules);
MCAPI std::string EntityTypeToString(ActorType, ActorTypeNamespaceRules);
MCAPI std::string EntityCanonicalName(ActorType);