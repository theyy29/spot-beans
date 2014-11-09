### To install:

wget -q -O - https://apt.mopidy.com/mopidy.gpg | sudo apt-key add -
sudo wget -q -O /etc/apt/sources.list.d/mopidy.list https://apt.mopidy.com/mopidy.list
sudo apt-get update
sudo apt-get install libspotify-dev build-essential python-dev python3-dev libffi-dev python-pip
sudo pip install --pre pyspotify