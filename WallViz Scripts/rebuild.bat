del /s /f /q C:\Users\wallvizpc\Desktop\ccvRun\*.*
for /f %%f in ('dir /ad /b C:\Users\wallvizpc\Desktop\ccvRun\') do rd /s /q C:\Users\wallvizpc\Desktop\ccvRun\%%f

xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\1\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\2\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\apps\addonsExamples\VS2010\bin" "C:\Users\wallvizpc\Desktop\ccvRun\3\" /S /Y

xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xmlBase" "C:\Users\wallvizpc\Desktop\ccvRun\1\data\xml\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml1\*.xml" "C:\Users\wallvizpc\Desktop\ccvRun\1\data\xml\" /S /Y

xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xmlBase" "C:\Users\wallvizpc\Desktop\ccvRun\2\data\xml\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml2\*.xml" "C:\Users\wallvizpc\Desktop\ccvRun\2\data\xml\" /S /Y

xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xmlBase" "C:\Users\wallvizpc\Desktop\ccvRun\3\data\xml\" /S /Y
xcopy "C:\Users\wallvizpc\workspace\ccv15\WallViz Scripts\xml3\*.xml" "C:\Users\wallvizpc\Desktop\ccvRun\3\data\xml\" /S /Y

@echo cd C:\Users\wallvizpc\Desktop\ccvRun\1\ >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo start C:\Users\wallvizpc\Desktop\ccvRun\1\CCV.exe >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo cd C:\Users\wallvizpc\Desktop\ccvRun\2\ >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo start C:\Users\wallvizpc\Desktop\ccvRun\2\CCV.exe >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo cd C:\Users\wallvizpc\Desktop\ccvRun\3\ >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"
@echo start C:\Users\wallvizpc\Desktop\ccvRun\3\CCV.exe >> "C:\Users\wallvizpc\Desktop\ccvRun\run.bat"