// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var LogIn$ReactTemplate = require("./LogIn.bs.js");
var Client$ReactTemplate = require("./Client.bs.js");
var AppStyle$ReactTemplate = require("./AppStyle.bs.js");
var LinkShare$ReactTemplate = require("./LinkShare.bs.js");
var GetUsername$ReactTemplate = require("./GetUsername.bs.js");
var CurrentlyPlaying$ReactTemplate = require("./CurrentlyPlaying.bs.js");

var userIcon = require("./img/user.png");

var component = ReasonReact.reducerComponent("App");

function make(_children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              self[/* state */1][/* auth */1].isLoggedIn("gmail").then((function (loginStatus) {
                        console.log(loginStatus);
                        Curry._1(self[/* send */3], /* SetLogInStatus */[loginStatus]);
                        return Promise.resolve(/* () */0);
                      })).catch((function (err) {
                      return Promise.resolve((console.log(err), /* () */0));
                    }));
              return /* () */0;
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              var match = self[/* state */1][/* isLoggedIn */0];
              return React.createElement("div", {
                          className: AppStyle$ReactTemplate.app
                        }, match ? React.createElement("div", undefined, ReasonReact.element(undefined, undefined, GetUsername$ReactTemplate.make(self[/* state */1][/* auth */1], (function (status) {
                                          return Curry._1(self[/* send */3], /* SetLogInStatus */[status]);
                                        }), /* array */[])), React.createElement("h1", {
                                    className: AppStyle$ReactTemplate.pageTitle
                                  }, "Welcome to SpotDJ"), ReasonReact.element(undefined, undefined, LinkShare$ReactTemplate.make(/* array */[])), ReasonReact.element(undefined, undefined, CurrentlyPlaying$ReactTemplate.make(/* array */[]))) : ReasonReact.element(undefined, undefined, LogIn$ReactTemplate.make(self[/* state */1][/* auth */1], (function (status) {
                                      return Curry._1(self[/* send */3], /* SetLogInStatus */[status]);
                                    }), /* array */[])));
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* isLoggedIn */false,
                      /* auth */Client$ReactTemplate.auth
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              return /* Update */Block.__(0, [/* record */[
                          /* isLoggedIn */action[0],
                          /* auth */state[/* auth */1]
                        ]]);
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

var Css = 0;

exports.Css = Css;
exports.userIcon = userIcon;
exports.component = component;
exports.make = make;
/* userIcon Not a pure module */
