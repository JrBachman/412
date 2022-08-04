//CHECK IF FOLDERS EXIST/CREATE FOLDER SECTION
  if(IsPathExist(watchFolder)){
    std::cout << "Watch PATH EXISTS" << std::endl;
  }
  else{
    std::cout << "Watch PATH DOESNT EXIST" << std::endl;
    std::system(("mkdir " + watchFolder).c_str());
  }
  if(IsPathExist(outputFolder)){
    std::cout << "Output PATH EXISTS" << std::endl;
  }
  else{
    std::cout << "Output PATH DOESNT EXIST" << std::endl;
    std::system(("mkdir " + outputFolder).c_str());
  }
//END CHECK IF FOLDERS EXIST SECTION