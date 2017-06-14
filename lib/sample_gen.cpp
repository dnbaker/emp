//.cpp:
#include "sample_gen.h"
namespace emp {
const char *classlvl_arr[31] {
    "no rank",
    "root",
    "superkingdom",
    "kingdom",
    "subkingdom",
    "superphylum",
    "phylum",
    "subphylum",
    "superclass",
    "class",
    "subclass",
    "infraclass",
    "cohort",
    "superorder",
    "order",
    "suborder",
    "infraorder",
    "parvorder",
    "superfamily",
    "family",
    "subfamily",
    "tribe",
    "subtribe",
    "genus",
    "subgenus",
    "species",
    "subspecies",
    "group",
    "subgroup",
    "varietas",
    "forma",
};

const std::unordered_map<std::string, ClassLevel> classlvl_map {
    {"no rank",      ClassLevel::NO_RANK},
    {"root",         ClassLevel::ROOT},
    {"superkingdom", ClassLevel::SUPERKINGDOM},
    {"kingdom",      ClassLevel::KINGDOM},
    {"subkingdom",   ClassLevel::SUBKINGDOM},
    {"superphylum",  ClassLevel::SUPERPHYLUM},
    {"phylum",       ClassLevel::PHYLUM},
    {"subphylum",    ClassLevel::SUBPHYLUM},
    {"superclass",   ClassLevel::SUPERCLASS},
    {"class",        ClassLevel::CLASS},
    {"subclass",     ClassLevel::SUBCLASS},
    {"infraclass",   ClassLevel::INFRACLASS},
    {"cohort",       ClassLevel::COHORT},
    {"superorder",   ClassLevel::SUPERORDER},
    {"order",        ClassLevel::ORDER},
    {"suborder",     ClassLevel::SUBORDER},
    {"infraorder",   ClassLevel::INFRAORDER},
    {"parvorder",    ClassLevel::PARVORDER},
    {"superfamily",  ClassLevel::SUPERFAMILY},
    {"family",       ClassLevel::FAMILY},
    {"subfamily",    ClassLevel::SUBFAMILY},
    {"tribe",        ClassLevel::TRIBE},
    {"subtribe",     ClassLevel::SUBTRIBE},
    {"genus",        ClassLevel::GENUS},
    {"subgenus",     ClassLevel::SUBGENUS},
    {"species",      ClassLevel::SPECIES},
    {"subspecies",   ClassLevel::SUBSPECIES},
    {"group",        ClassLevel::GROUP},
    {"subgroup",     ClassLevel::SUBGROUP},
    {"varietas",     ClassLevel::VARIETAS},
    {"forma",        ClassLevel::FORMA},
};


} // namespace emp
