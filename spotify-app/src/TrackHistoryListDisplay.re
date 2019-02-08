open Emotion;
open ReactMotionBinding;

let imagePreviewWidth = 200;
let imagePreviewContainer = 64;
let imagePreviewTotalWidth = imagePreviewWidth + imagePreviewContainer;

let wrapper = [%css
  [
    display(`flex),
    height(`px(450)),
    position(`relative),
    top(`px(0)),
    zIndex(-1),
    width(`vw(100.)),
    margin(`auto),
    perspective(`px(1000)),
  ]
];

let trackStyle = [%css
  [
    color(`hex("fff")),
    width(`px(200)),
    height(`px(250)),
    position(`absolute),
    margin(`px(16)),
    flexShrink(0.),
    transformStyle(`preserve3d),
    zIndex(2),
  ]
];

let trackStyleHidden = [%css
  [
    backgroundColor(`hex("123")),
    color(`hex("000")),
    width(`px(200)),
    height(`px(300)),
    position(`relative),
    margin(`px(32)),
    flexShrink(0.),
    right(`px(0)),
  ]
];

type state = {
  focusedIdx: int,
  scrollLeft: float,
  initialScrollLeft: float,
  trackList: array(string),
};

type action =
  | UpdateFocusedId
  | HandleXScroll(float)
  | UpdateTrackListAndScrollPosition(array(string), float);

let component = ReasonReact.reducerComponent("TrackhistoryListDisplay");

let getAnimationStyleValue = (trackList, currentScroll) => {
  let returnFunction = () =>
    trackList
    |> Array.mapi((idx, _value) => {
         let centerDiff =
           Utils.clientWidth
           *. 0.5
           -. float_of_int(Array.length(trackList))
           *. float_of_int(imagePreviewTotalWidth)
           *. 0.5
           +. 100.0;

         let offSetX =
           currentScroll
           +. float_of_int(idx)
           *. float_of_int(imagePreviewTotalWidth)
           +. centerDiff;

         let center =
           offSetX
           -. (
             Utils.clientWidth
             *. 0.50
             -. float_of_int(imagePreviewTotalWidth)
             *. 0.50
           );

         let isFocused =
           abs_float(center) < float_of_int(imagePreviewTotalWidth) *. 0.40;

         switch (isFocused) {
         | false =>
           let scale = 1.0;
           let rotate = (-40.0);
           let opacity = 0.3;
           let returnStyle = {
             x: spring(offSetX, gentle),
             scale: spring(scale, gentle),
             rotate: spring(rotate, gentle),
             opacity: spring(opacity, gentle),
           };

           returnStyle;
         | true =>
           let scale = idx === Array.length(trackList) - 1 ? 1.0 : 1.3;
           let rotate = 0.0;
           let opacity = 1.0;
           let returnStyle = {
             x: spring(offSetX, gentle),
             scale: spring(scale, gentle),
             rotate: spring(rotate, gentle),
             opacity: spring(opacity, gentle),
           };

           returnStyle;
         };
       });

  returnFunction;
};

let trackAnimationStyle = (xVal, scale, rotate, opacity) => {
  let xVal = string_of_float(xVal);
  let scale = string_of_float(scale);
  let rotate = string_of_float(rotate);
  let zIndex = opacity > 0.5 ? "10" : "";
  let opacity = string_of_float(opacity);

  let transform =
    "translate("
    ++ xVal
    ++ "0px, 0px) scale("
    ++ scale
    ++ "0) rotateY("
    ++ rotate
    ++ "0deg)";

  ReactDOMRe.Style.make(~transform, ~opacity, ~zIndex, ());
};

let make =
    (
      ~trackList,
      ~currentSongName,
      ~currentArtistName,
      ~currentIsPlaying,
      ~currentProgressPct,
      ~currentAlbumImageUrl,
      ~currentIsFirstSong,
      _children,
    ) => {
  ...component,
  initialState: () => {
    focusedIdx: 0,
    scrollLeft:
      float_of_int(Array.length(trackList))
      *. 0.5
      *. float_of_int(imagePreviewTotalWidth)
      *. (-1.0),
    initialScrollLeft:
      float_of_int(Array.length(trackList))
      *. 0.5
      *. float_of_int(imagePreviewTotalWidth)
      *. (-1.0),
    trackList,
  },
  reducer: (action, state) =>
    ReasonReact.(
      switch (action) {
      | UpdateFocusedId => Update({...state, focusedIdx: state.focusedIdx})
      | HandleXScroll(dx) =>
        let boundary =
          float_of_int(imagePreviewTotalWidth * Array.length(trackList))
          /. 2.;
        Js.log2("boundary:", boundary);
        Js.log2("dx:", dx);
        let scrollLeft =
          state.scrollLeft
          -. dx
          |> min(boundary -. 200.0, _)
          |> max(boundary *. (-1.0), _);
        Js.log2("scrollLeft:", scrollLeft);
        Update({...state, scrollLeft});
      | UpdateTrackListAndScrollPosition(trackList, scrollLeft) =>
        Update({...state, trackList, scrollLeft})
      }
    ),
  didMount: self =>
    Utils.addDocumentMouseWheelEventListener(
      "mousewheel",
      event => {
        Webapi.Dom.WheelEvent.preventDefault(event);
        Webapi.Dom.WheelEvent.stopPropagation(event);
        let dx = Webapi.Dom.WheelEvent.deltaX(event);

        self.send(HandleXScroll(dx));
      },
      false,
    ),
  willUpdate: ({oldSelf, newSelf}) =>
    if (Array.length(newSelf.state.trackList) !== Array.length(trackList)) {
      let boundary =
        float_of_int(
          imagePreviewTotalWidth * Array.length(newSelf.state.trackList),
        )
        /. 2.;
      let scrollLeft =
        switch (boundary +. newSelf.state.scrollLeft < 50.) {
        | false =>
          newSelf.state.scrollLeft
          +. float_of_int(imagePreviewTotalWidth)
          *. 0.5
        | true =>
          float_of_int(Array.length(trackList))
          *. 0.5
          *. float_of_int(imagePreviewTotalWidth)
          *. (-1.0)
        };

      newSelf.send(UpdateTrackListAndScrollPosition(trackList, scrollLeft));
    } else {
      ();
    },
  render: self =>
    ReasonReact.(
      <div>
        <StaggeredMotion
          key={Array.length(trackList) |> string_of_int}
          className="motion"
          getAimationStyleValue={
            getAnimationStyleValue(trackList, self.state.scrollLeft)
          }
          defaultStyles={
            trackList
            |> Array.map(_trackId => {
                 let offSetX = Utils.clientWidth +. 300.0;
                 {x: offSetX, scale: 1.0, rotate: (-40.0), opacity: 0.3};
               })
          }>
          {
            styleValue =>
              <div className=wrapper id="trackListWrapper">
                {
                  trackList
                  |> Array.mapi((idx, trackId) =>
                       <div
                         key={string_of_int(idx)}
                         className={Cn.make([trackStyle])}
                         id={"number_" ++ string_of_int(idx)}
                         style={
                           trackAnimationStyle(
                             styleValue[idx].x,
                             styleValue[idx].scale,
                             styleValue[idx].rotate,
                             styleValue[idx].opacity,
                           )
                         }>
                         {
                           idx === Array.length(trackList) - 1 ?
                             <CurrentlyPlaying
                               songName=currentSongName
                               artistName=currentArtistName
                               isPlaying=currentIsPlaying
                               progressPct=currentProgressPct
                               albumImageUrl=currentAlbumImageUrl
                               isFirstSong=currentIsFirstSong
                             /> :
                             <GetTrackInfoQuery trackId>
                               ...{
                                    ({songName, artistName, albumImageUrl}) =>
                                      <SimpleTrack
                                        songName
                                        artistName
                                        albumImageUrl
                                        newestHistory={
                                          idx === Array.length(trackList) - 2 ?
                                            true : false
                                        }
                                        isCurrentTrack=false
                                        idx
                                      />
                                  }
                             </GetTrackInfoQuery>
                         }
                       </div>
                     )
                  |> ReasonReact.array
                }
              </div>
          }
        </StaggeredMotion>
      </div>
    ),
};
