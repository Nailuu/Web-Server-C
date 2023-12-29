require 'socket'
starttime = Process.clock_gettime(Process::CLOCK_MONOTONIC)

s = TCPSocket.new 'localhost', 8080

s.write("GET /files/map.txt HTTP/1.1\n")

s.each_line do |line|
  #puts line
end

s.close
endtime = Process.clock_gettime(Process::CLOCK_MONOTONIC)
elapsed = endtime - starttime
puts "Elapsed: #{elapsed}"
