my-C-Logger
Usage:
```c++
  LLogger logger;
  logger.EnableInfo(true);
  logger.Info()<<"to console test";
  ofstream file("test.txt");
  logger.Redirect(file);
  logger.Err()<<"to file test"<<endl;
  file.close();
  system("pause");
```
