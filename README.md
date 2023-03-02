# ![](https://github.com/roboxerox/SerialCOM/blob/main/images/Connections.png) SerialCOM
Qt based UI application to send or receive data over serial port on platform Unix/Linux.


### Use the following commands to install the basic requirements for building Qt applications

### Debian/Ubuntu (apt-get)

	sudo apt-get install build-essential libgl1-mesa-dev
	
For More Help [https://wiki.qt.io/Install_Qt_5_on_Ubuntu]

### Fedora/RHEL/CentOS (yum)

	sudo yum groupinstall "C Development Tools and Libraries"
	sudo yum install mesa-libGL-devel

### openSUSE (zypper)

	sudo zypper install -t pattern devel_basis


### Step to compile with qt

- Open QtCreator
- Open Project -> select file (SerialCOM.pro)
- Build and run

### Step to compile with cmake/make

	git clone https://github.com/roboxerox/SerialCOM.git
	cd SerialCOM
	mkdir c_build  && cd  c_build
	cmake ..
	make
	
### Step to compile with qmake/make

	git clone https://github.com/roboxerox/SerialCOM.git
	cd SerialCOM
	mkdir q_build  && cd  q_build
	qmake ..
	make




## License
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
