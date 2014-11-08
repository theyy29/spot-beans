<!doctype html>
<html lang="en">
<head>
    <title>Something</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <link href="/css/bootstrap.min.css" rel="stylesheet"/>
    <link href="/css/style.css" rel="stylesheet">
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
        <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>

<body>
    <div id="playlist" class="">
        <div id="playlist-controls">
            <span class="glyphicon glyphicon-repeat playlist-control playlist-control-backward"></span>
            <span class="glyphicon glyphicon-random playlist-control playlist-control-backward"></span>
            <br>
            <span class="glyphicon glyphicon-step-backward playlist-control playlist-control-backward"></span>
            <span class="glyphicon glyphicon-step-forward playlist-control playlist-control-forward"></span>
        </div>
        <div id="playlist-list">
            <ul class="nav nav-pills nav-stacked" role="tablist">
                <li class="active"><a href="#">Playlist1</a></li>
                <li><a href="#">Playlist2</a></li>
                <li><a href="#">Playlist3</a></li>
                <li><a href="#">Playlist4</a></li>
            </ul>
        </div>

    </div>

    <div id="song-content" class="">

        <div class="container-fluid">
            <div class="row">
                <div id="song-info" class="col-sm-6">
                    <div class="row">
                        <div class="col-sm-3">
                            <img class="img-responsive song-img" src="/img/dpa-tmp.jpg" alt="Album Cover"> <!-- Obviously a placeholder. -->
                        </div>
                        <div class="song-info-text">
                            <p>Song Name</p>
                            <p>Artist Name</p>
                            <p>Album Name</p>
                        </div>
                    </div>
                </div>
                <div id="song-controls" class="col-sm-6">
                    <span class="glyphicon glyphicon-step-backward song-control song-control-backward"></span>
                    <span class="glyphicon glyphicon-pause song-control song-control-pause"></span>
                    <span class="glyphicon glyphicon-step-forward song-control song-control-forward"></span>
                    <div class="progress">
                        <div class="progress-bar" role="progressbar" aria-valuenow="60" aria-valuemin="0" aria-valuemax="100" style="width: 60%;">
                        </div>
                    </div>
                </div>
            </div>
        </div>

    </div>

    <div id="playlist-content" class="">

        <div id="playlist-song-list">
            <ul class="nav nav-pills nav-stacked" role="tablist">
                <li class="playlist-sort">
                    <span class="song-play"></span>
                    <span class="song-name">Song</span>
                    <span class="song-artist">Artist</span>
                    <span class="song-album">Album</span>
                    <span class="song-time">Time</span>
                </li>
                <li>
                    <span class="glyphicon glyphicon-play song-play"></span>
                    <span class="song-name">Song2</span>
                    <span class="song-artist">Artist name</span>
                    <span class="song-album">Album name</span>
                    <span class="song-time">time</span>
                </li>
                <li>
                    <span class="glyphicon glyphicon-play song-play"></span>
                    <span class="song-name">Song asdasdsadadas ad sada</span>
                    <span class="song-artist">Artist name</span>
                    <span class="song-album">Album name</span>
                    <span class="song-time">time</span>
                </li>
            </div>
        </div>

    </div>

    <script src="js/jquery-2.1.1.min.js"></script>
    <script src="js/js.js"></script>

</body>
</html>