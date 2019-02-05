/* require css file for side effect only */
[@bs.val] external requireCSS: string => unit = "require";

/* require an asset (eg. an image) and return exported string value (image URI) */
[@bs.val] external requireAssetURI: string => string = "require";

module Window = {
  [@bs.val] [@bs.scope ("window", "location")] external href: string = "href";

  [@bs.val] [@bs.scope ("window", "location")]
  external protocol: string = "protocol";

  [@bs.val] [@bs.scope ("window", "location")] external host: string = "host";
};

[@bs.val] [@bs.scope ("window", "sessionStorage")]
external sessionStorageSetItem: (string, string) => unit = "setItem";

[@bs.val] [@bs.scope ("window", "sessionStorage")]
external sessionStorageGetItem: string => Js.null(BsUuid.Uuid.V4.t) =
  "getItem";

[@bs.val] [@bs.scope ("window", "localStorage")]
external localStorageSetItem: (string, string) => unit = "setItem";

[@bs.val] [@bs.scope ("window", "localStorage")]
external localStorageGetItem: string => Js.null(BsUuid.Uuid.V4.t) = "getItem";

let getImageUrl = (~images, ~defaultImage) => {
  let urls =
    Js.Array.map(
      image =>
        switch (image##url) {
        | Some(url) => url
        | None => ""
        },
      images,
    )
    |> Js.Array.filter(url => url !== "");

  Array.length(urls) > 1 ?
    urls[1] : Array.length(urls) == 1 ? urls[0] : defaultImage;
};

[@bs.val] [@bs.scope "window"]
external windowOpen: (string, string, string) => unit = "open";

[@bs.val] [@bs.scope ("window", "navigator", "clipboard")]
external writeText: string => Js.Promise.t(string) = "writeText";

type element;

[@bs.val] [@bs.scope "document"]
external getElementById: string => element = "getElementById";

[@bs.val] [@bs.scope "document"]
external execCommand: string => bool = "execCommand";

[@bs.send] external selectElement: (element, unit) => unit = "select";
