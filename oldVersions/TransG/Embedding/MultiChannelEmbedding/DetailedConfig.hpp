#pragma once
#include "Import.hpp"
#include "ModelConfig.hpp"


const Dataset FB15K("FB15K", "FB15K/", "train.txt", "dev.txt", "test.txt", true);
const Dataset FB13("FB13", "FB13/", "train.txt", "dev.txt", "test.txt", false);
const Dataset WN11("WN11", "WN11/", "train.txt", "dev.txt", "test.txt", false);
const Dataset WN18("WN18", "WN18/", "train.txt", "dev.txt", "test.txt", true);
// const Dataset Wordnet("Wordnet", "/home/bookman/data/Knowledge Embedding/Wordnet/", "train.txt", "dev.txt", "test.txt", false);
// const Dataset Freebase("Freebase", "/home/bookman/data/Knowledge Embedding/Freebase/", "train.txt", "dev.txt", "test.txt", false);
const string report_path = "Report/";
const string semantic_vfile_FB15K = "FB15K/topics.bsd";
const string semantic_tfile_FB15K = "FB15K/description.txt";
const string semantic_vfile_WN18 = "WN18/topics.bsd";
const string semantic_tfile_WN18 = "WN18/descriptions.txt";
const string type_file_FB15K = "FB15K/type.txt";
// const string type_file_FB15KZS = "/home/bookman/data/Knowledge Embedding/FB15KZS/type.txt";
// const string triple_zeroshot_FB15K = "/home/bookman/data/Knowledge Embedding/FB15KZS/zeroshot.txt";
// const string semantic_vfile_FB15KZS = "/home/bookman/data/Knowledge Embedding/FB15KZS/topics.bsd";
// const string semantic_tfile_FB15KZS = "/home/bookman/data/Knowledge Embedding/FB15KZS/description.txt";
