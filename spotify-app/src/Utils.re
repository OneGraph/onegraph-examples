/* require css file for side effect only */
[@bs.val] external requireCSS: string => unit = "require";

/* require an asset (eg. an image) and return exported string value (image URI) */
[@bs.val] external requireAssetURI: string => string = "require";

[@bs.val] [@bs.scope ("window", "location")]
external windowHref: string = "href";

[@bs.val] [@bs.scope ("window", "sessionStorage")]
external sessionStorageSetItem: (string, string) => unit = "setItem";

[@bs.val] [@bs.scope ("window", "sessionStorage")]
external sessionStorageGetItem: string => Js.null(BsUuid.Uuid.V4.t) =
  "getItem";

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
