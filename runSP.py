import os
import shutil
import subprocess
import argparse
import signal
from customized_timeout import TimeoutException,TimeoutError


TestsSP = \
  [
  ("SP",[("SPpdrabc","PdrAbc"), ("SPpdrchc","PdrChc"), ("SPgrain","Grain"), ("SPrelchc","RelChc"), ("SPcvc4sy","Cvc4Sy") ])
  ]

def getNumbers(fin):
  res = fin.readline()
  total_time = float(fin.readline())
  cegar = int(fin.readline())
  syn_time = float(fin.readline())
  eq_time = float(fin.readline())
  return res, cegar, syn_time, eq_time, total_time
  

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

def RunTests(tests, timeout, total):
  cwd = os.getcwd()
  idx = 1
  for directory, test_list in tests:
    base_dir = os.path.join(cwd, directory)
    test_prg_dir = os.path.join(base_dir,"build")
    #print test_list
    for prg,outDir in test_list:
      # clear result
      test_result_file = os.path.join(base_dir,"verification/"+outDir+'/result-stat.txt')
      if os.path.exists(test_result_file):
          os.remove(test_result_file)
      full_prg = os.path.join(test_prg_dir, prg)
      
      print '--------------------------'
      print '|        Job: (%3d/%3d)  |' % (idx, total)
      print '--------------------------'
      print 'Run:', full_prg
      idx += 1
      if not os.path.exists( full_prg ):
        print full_prg,'not available'

      os.chdir(test_prg_dir)
      #result_log = open('running_result.log','w')
      process = subprocess.Popen(['./' + prg,str(timeout)], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
      print 'PID:', process.pid
      print 'Method:',outDir
      #os.setpgid(process.pid, process.pid)
      #process.getpgid(process.pid)
      try:
        with TimeoutException(int(timeout)+5):
            process.communicate()
      except KeyboardInterrupt:
        print 'Try killing subprocess...',
        try:
          process.terminate()
          print 'Done'
        except OSError:
          print 'Unable to kill'
        process.wait()     
        
      except TimeoutError:
        print 'Try killing subprocess...',
        try:
          process.terminate()
          os.system('pkill cvc4')
          print 'Done'
        except OSError:
          print 'Unable to kill'
        process.wait()     
          
      if os.path.exists(test_result_file):
        with open(test_result_file) as fin:
          res, cegar_iter, syn_time, eq_time,total_time = getNumbers(fin)
          if outDir == 'RelChc':
            print 
            print '--------------------------'
            print '|       Result           |'
            print '--------------------------'
            print 'Process:    ',res
            print 't(total)  =',total_time
            print '--------------------------'
            print
            
          else:
            print 
            print '--------------------------'
            print '|       Result           |'
            print '--------------------------'
            print 'Process:    ',res
            print '#(iter)    =',cegar_iter
            print 't(syn)     =',syn_time
            print 't(eq)      =',eq_time
            print 't(syn+eq)  =',syn_time+eq_time
            print '--------------------------'
            print
      else:
        print '--------------------------'
        print '|       Result           |'
        print '--------------------------'
        print 'skipped'
        print '--------------------------'
        print
      # if exists
      # execute with timeout

parser = argparse.ArgumentParser(description='Run Experiments on Redundant Counters (RC)')
parser.add_argument('-t','--timeout',
                    default=60*10,
                    help='The time limit in seconds')
                    
args = parser.parse_args()
testset = TestsSP

print '--------------------------'
print '|        Jobs            |'
print '--------------------------'
print 'Will launch (%d) jobs ' % len(testset[0][1])
print 'Time-out limit (sec): ',args.timeout
print '--------------------------'
print 

ClearVerifOutput(testset)
RunTests(testset, args.timeout, len(testset[0][1]))
#ClearVerifOutput(TestsAll)
    
    
