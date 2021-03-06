# covenant-iot

## Install necessary software:

### 1. Install npm and nodejs

nodejs: https://nodejs.org/en/
node (update): `npm install npm@latest -g`

To check if successful, `npm -v`, `node -v`.

### 2. Install git
If in Linux-based systems:

Type `sudo add-apt-repository ppa:git-core/ppa`

Type `sudo apt-get update`

Type `sudo apt-get install -y git`

If not linux: Follow steps here: https://git-scm.com/book/en/v2/Getting-Started-Installing-Git

### 3. Install Ionic

Type `npm install -g ionic cordova`
Setup video here: https://www.youtube.com/watch?v=BDDUpP5SUA8

Note: You may encounter problems regarding CORS. If so, install this add-on to your browser: https://chrome.google.com/webstore/detail/allow-control-allow-origi/nlfbmbojpeacfghkpbjhddihlkkiljbi?hl=en

### 4. Install Rails

If on Windows: use [RailsInstaller](http://railsinstaller.org/en)

If on Linux:
1. Install `ruby` and `rubygems` (or an equivalent package)
2. `gem install rails`
3. `gem install bundler`
4. Install `sqlite` (or an equivalent package)
5. Install `ruby-bundler` (or an equivalent package)

## Setup:

1. Create folder named "CS145 capstone" (or any other name)
2. Open cmd/terminal in ~/CS145 capstone then type `git clone https://github.com/cadaroya/covenant-iot.git`
3. cd to ionic-app then type `npm install`
    - If this doesn't work, use `npm install --no-bin-links`
4. cd to rails-server then type `bundle install` and `rails db:migrate`

## Running the project

1. cd to rails-server then type `rails s` to start the back-end server (port 3000)
2. cd to ionic-app then type `ionic serve` to start the mobile-dev environment (port 8100)
    - If you get [this error](https://github.com/sass/node-sass/issues/1764), run `sudo npm rebuild node-sass --no-bin-links`

## Additional:
1. For development, I suggest using visual code as text editor https://code.visualstudio.com/ because it has git functionalities integrated into it.

2. Ionic tutorial: https://www.youtube.com/watch?v=qs2n_poLarc&t=2548s

3. Rails tutorial: https://www.youtube.com/watch?v=QojnRc7SS9o&t=1159s

v-0.0.2
