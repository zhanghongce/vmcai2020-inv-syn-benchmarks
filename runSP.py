import os
import shutil
import subprocess


TestsSP = \
  [
  ("SP",[("SPpdrabc","PdrAbc"), ("SPpdrchc","PdrChc"), ("SPgrain","Grain"), ("SPrelchc","RelChc"), ("SPcvc4sy","Cvc4Sy") ])
  ]


def ClearVerifOutput(tests):
  cwd = os.getcwd()
  for directory, test_list in tests:
    if directory == 'GB':
      continue
    base_dir = os.path.join(cwd, directory)
    for prg,outDir in test_list:
      result_dir = os.path.join(base_dir,"verification/"+outDir)
      if os.path.exists(result_dir):
        shutil.rmtree(result_dir)
      os.mkdir(result_dir)

def RunTests(tests, timeout):
  cwd = os.getcwd()
  for directory, test_list in tests:
    base_dir = os.path.join(cwd, directory)
    test_prg_dir = os.path.join(base_dir,"build")
    print test_list
    for prg,outDir in test_list:
      # clear result
      test_result_file = os.path.join(base_dir,"verification/"+outDir+'/result-stat.txt')
      if os.path.exists(test_result_file):
          os.remove(test_result_file)
      full_prg = os.path.join(test_prg_dir, prg)
      print 'Run:', full_prg
      if not os.path.exists( full_prg ):
        print full_prg,'not available'

      os.chdir(test_prg_dir)
      #result_log = open('running_result.log','w')
      process = subprocess.Popen(['./' + prg,str(timeout)], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
      print 'PID:', process.pid
      #os.setpgid(process.pid, process.pid)
      #process.getpgid(process.pid)
      process.communicate()
      if os.path.exists(test_result_file):
        with open(test_result_file) as fin:
          print outDir,'\n--------------------------'
          print fin.read()
          print '--------------------------'
      else:
        print test_result_file, 'not available'
      # if exists
      # execute with timeout


ClearVerifOutput(TestsSP)
RunTests(TestsSP, 3600)
#ClearVerifOutput(TestsAll)
    
    
