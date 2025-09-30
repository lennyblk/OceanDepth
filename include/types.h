#ifndef TYPES_H
#define TYPES_H

// ENUM DE BASE =================================================================================================
typedef enum {
    CREATURE_KRAKEN,
    CREATURE_SHARK,
    CREATURE_JELLYFISH,
    CREATURE_SWORDFISH,
    CREATURE_GIANT_CRAB,
    CREATURE_COUNT  
} CreatureType;

typedef enum {
    EFFECT_NONE,
    EFFECT_PARALYSIS,
    EFFECT_POISON,
    EFFECT_BLEED,
    EFFECT_STUN
} SpecialEffect;

typedef enum {
    ITEM_OXYGEN_CAPSULE,
    ITEM_HEALTH_KIT,
    ITEM_STIMULANT,
    ITEM_ANTIDOTE,
    ITEM_COUNT
} ItemType;

typedef enum {
    ZONE_SURFACE,
    ZONE_REEF,
    ZONE_WRECK,
    ZONE_KELP_FOREST,
    ZONE_CAVE,
    ZONE_ABYSS,
    ZONE_COUNT
} ZoneType;

typedef enum {
    RARITY_COMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
} Rarity;

typedef enum {
    SKILL_EXTENDED_BREATH,
    SKILL_ELECTRIC_DISCHARGE,
    SKILL_MARINE_COMMUNICATION,
    SKILL_WATER_VORTEX,
    SKILL_COUNT
} SkillType;

typedef enum {
    WEAPON_RUSTY_HARPOON,
    WEAPON_STEEL_HARPOON,
    WEAPON_ELECTRIC_HARPOON,
    WEAPON_LASER_HARPOON,
    WEAPON_COUNT
} WeaponType;

// STRUCTURES DE CRÉATURE =======================================================================================================
typedef struct {
    int id;                          
    char name[30];                   
    CreatureType type;               
    
    int hp_max;                      
    int hp_current;                  
    int attack_min;                  
    int attack_max;                  
    int defense;                     
    int speed;                       
    
    SpecialEffect special_effect;    
    int effect_duration;             
    int is_alive;                    
} Creature;

// Arme (harpon)
typedef struct {
    WeaponType type;
    char name[40];
    int attack_min;
    int attack_max;
    int oxygen_cost_per_attack;       
    int defense_ignore;              
    Rarity rarity;
} Weapon;

// Armure (combinaison)
typedef struct {
    char name[40];
    int defense;
    int oxygen_cost_per_turn;        
    Rarity rarity;
} Armor;

// Objet consommable
typedef struct {
    ItemType type;
    char name[40];
    int quantity;
    // effet de l'objet
    int hp_restore;                  
    int oxygen_restore;
    int fatigue_reduce;
    int removes_effect;
} Item;

// STRUCTURE DU JOUEUR =======================================================================================================

typedef struct {
    char name[50];                   
    int level;                       
    int experience;                  
    
    int hp;                          
    int max_hp;                      
    int oxygen;                      
    int max_oxygen;                  
    
    int x;                           
    int y;                           
    int current_zone;              
    int zones_unlocked; 
    
    // Monnaie
    int pearls;                      
    
    Weapon equipped_weapon;          
    Armor equipped_armor;            
    
    Item inventory[4];
    int inventory_count;             
    
    int is_paralyzed;                
    int is_poisoned;                 
    int paralysis_turns_left;        
    int poison_turns_left;           
} Player;

// STRUCTURE SKILL =======================================================================================================

typedef struct {
    SkillType type;
    char name[40];
    char description[100];
    int oxygen_cost;                 
    int cooldown_max;                
    int cooldown_current;            
    int is_unlocked;                 
    
    // Effets de la compétence
    int damage_min;                  
    int damage_max;                  
    int oxygen_restore;              
    int hits_all_enemies;            
    int pacifies_enemy;              
    int slows_enemies;               
} Skill;

// STRUCTURE ZONE =======================================================================================================
typedef struct {
    int id;
    char name[40];
    int cleared;
    int available;
    int monsters_count;
    CreatureType monsters[10];
} Destination;

typedef struct {
    int id;                          
    ZoneType type;                   
    char name[40];                   
    int depth;                       
    int is_discovered;               
    int is_current_location;         
    
    char current_strength[20];       
    char visibility[20];             
    int temperature;   
   
    Destination destinations[4];
    
    Creature creatures[4];           
    int creature_count;              
    
    // Propriétés spéciales
    int can_save;                    
    int has_shop;                    
    int oxygen_cost_to_enter;        
} Zone;

// STRUCTURE MAP =======================================================================================================

typedef struct {
    Zone zones[10];                  
    int zone_count;                  
    int current_zone_index;          
} Map;

// STRUCTURE SAVE =======================================================================================================

typedef struct {
    Player player;                  
    Map map;                        
    int game_time;                  
    int save_version;               
} SaveData;

// STRUCTURE COMBAT =======================================================================================================

typedef struct {
    Player* player;                  
    Creature* creatures;             
    int creature_count;              
    int turn_number;                 
    int combat_active;               
} CombatState;

// FONCTIONS DE BASE =======================================================================================================
void initialize_game(Player* player, Map* map);

#endif