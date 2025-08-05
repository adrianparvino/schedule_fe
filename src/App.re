[@mel.module "./index.css"] external _css: unit;

[@mel.scope ("import", "meta", "env")]
external backendUrl: string = "VITE_BACKEND_URL";

module RTF = {
  type t;
  type options = {
    localeMatcher: option(string),
    numberingSystem: option(string),
    style: option(string),
    numeric: option(string),
  };

  [@mel.scope "Intl"] [@mel.new]
  external make: unit => t = "RelativeTimeFormat";

  [@mel.scope "Intl"] [@mel.new]
  external makeWithLocale: option(string) => t = "RelativeTimeFormat";

  [@mel.scope "Intl"] [@mel.new]
  external makeWithLocaleAndOptions: (option(string), options) => t =
    "RelativeTimeFormat";

  [@mel.send] external format: (t, float, string) => string = "format";
};

module Courses = {
  open Melange_json.Primitives;

  [@deriving of_json]
  type course = {
    courseCode: string,
    [@json.option] [@json.key "MON"]
    mon: option(array(array(int))),
    [@json.option] [@json.key "TUE"]
    tue: option(array(array(int))),
    [@json.option] [@json.key "WED"]
    wed: option(array(array(int))),
    [@json.option] [@json.key "THU"]
    thu: option(array(array(int))),
    [@json.option] [@json.key "FRI"]
    fri: option(array(array(int))),
    [@json.option] [@json.key "SAT"]
    sat: option(array(array(int))),
    [@json.option] [@json.key "SUN"]
    sun: option(array(array(int))),
  };

  [@deriving of_json]
  type t = {courses: array(course)};

  [@react.component]
  let make = (~courses: t) => {
    let normalize = (time: int) => {
      let hour = time / 100;
      let min = time mod 100;
      let odd =
        if (min >= 30) {
          1;
        } else {
          0;
        };
      100 * hour + 50 * odd;
    };

    let flattened =
      courses.courses
      ->Belt.Array.flatMap(course => {
          let mon =
            course.mon
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  1,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let tue =
            course.tue
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  2,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let wed =
            course.wed
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  3,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let thu =
            course.thu
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  4,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let fri =
            course.fri
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  5,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let sat =
            course.sat
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  6,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );
          let sun =
            course.sun
            ->Belt.Option.getWithDefault([||])
            ->Belt.Array.map(range =>
                (
                  course.courseCode,
                  7,
                  normalize(range[0]),
                  normalize(range[1]),
                )
              );

          [|mon, tue, wed, thu, fri, sat, sun|]->Belt.Array.concatMany;
        });

    let minTime =
      Belt.Array.reduce(flattened, 600, (acc, (_, _, start, end_)) =>
        min(acc, min(start, end_))
      );
    let maxTime =
      Belt.Array.reduce(flattened, 1800, (acc, (_, _, start, end_)) =>
        max(acc, max(start, end_))
      );

    let minTime = minTime / 100 * 100;
    let maxTime = (maxTime - 50) / 100 * 100;

    let times = Belt.Array.rangeBy(minTime, maxTime, ~step=100);
    let slots = 2 * Belt.Array.length(times);

    let pretty = (time: int) => {
      let hour = time / 100;
      let min = time mod 100;
      let afternoon = hour >= 12;

      let hour =
        if (afternoon) {
          hour - 12;
        } else {
          hour;
        };
      let hour =
        if (hour == 0) {
          12;
        } else {
          hour;
        };
      let hour = hour |> string_of_int;
      let min =
        if (min >= 50) {
          "30";
        } else {
          "00";
        };
      let indicator = if (afternoon) {"PM"} else {"AM"};

      hour ++ ":" ++ min ++ " " ++ indicator;
    };

    let toElement = ((courseCode, day, start, end_)) => {
      let len = (end_ - start) / 50;
      let start = start - minTime;

      let start = start / 50 + 1;

      let start = start |> string_of_int;
      let len = len |> string_of_int;
      let day = day |> string_of_int;
      <div
        className={
          [|
            "bg-green-800",
            "p-2",
            "m-px",
            "rounded-xs",
            "text-white",
            "font-bold",
            "row-start-" ++ start,
            "row-span-" ++ len,
            "col-start-" ++ day,
          |]
          |> Js.Array.join(~sep="  ")
        }>
        {courseCode |> React.string}
      </div>;
    };

    <div
      className={
        "grid items-stretch grid-cols-cal grid-rows-cal"
        ++ (slots |> string_of_int)
      }>
      {[|
         "Monday",
         "Tuesday",
         "Wednesday",
         "Thursday",
         "Friday",
         "Saturday",
         "Sunday",
       |]
       ->Belt.Array.mapWithIndex((i, day) =>
           <div
             key=day
             className={
               [|
                 "row-start-1",
                 "border",
                 "col-start-" ++ (i + 2 |> string_of_int),
               |]
               |> Js.Array.join(~sep="  ")
             }>
             {day |> React.string}
           </div>
         )
       ->React.array}
      {times
       ->Belt.Array.mapWithIndex((i, time) => {
           <div
             className={
               [|
                 "col-start-1",
                 "flex",
                 "justify-center",
                 "items-center",
                 "px-2",
                 "row-span-2",
                 "border",
                 "border-b-0",
                 "row-start-" ++ (2 * i + 2 |> string_of_int),
               |]
               |> Js.Array.join(~sep="  ")
             }>
             {time |> pretty |> React.string}
           </div>
         })
       ->React.array}
      <div
        className="grid grid-rows-subgrid grid-cols-subgrid col-start-2 col-end-[-1] row-start-2 row-end-[-1]">
        {flattened->Belt.Array.map(toElement)->React.array}
      </div>
    </div>;
  };
};

module MainApp = {
  let analyze = str => {
    Js.Promise.(
      Fetch.(
        RequestInit.make(
          ~method_=Post,
          ~headers=HeadersInit.make({"Content-Type": "application/text"}),
          ~body=BodyInit.make(str),
          (),
        )
        |> fetchWithInit(backendUrl)
      )
      |> then_(Fetch.Response.json)
    );
  };

  [@react.component]
  let make = () => {
    let (schedule, setSchedule) =
      React.useReducer((_, newValue) => newValue, [||]);
    let (input, setInput) = React.useReducer((_, newValue) => newValue, "");
    let courses = {raw|{
      "courses": [
        {
          "courseCode": "CMSC 123",
          "MON": [[1330, 1500]],
          "THU": [[1330, 1500]]
        },
        {
          "courseCode": "CMSC 123 LAB",
          "MON": [[1500, 1630]],
          "THU": [[1500, 1630]]
        },
        {
          "courseCode": "MATH 121",
          "MON": [[730, 900]],
          "THU": [[730, 900]]
        },
        {
          "courseCode": "MATH 131",
          "MON": [[1030, 1200]],
          "THU": [[1030, 1200]]
        },
        {
          "courseCode": "MATH 161",
          "TUE": [[1330, 1500]],
          "FRI": [[1330, 1500]]
        },
        {
          "courseCode": "SCIENCE 10",
          "MON": [[900, 1030]],
          "THU": [[900, 1030]]
        },
        {
          "courseCode": "STAT 136",
          "TUE": [[800, 900]],
          "FRI": [[800, 900]]
        },
        {
          "courseCode": "STAT 136 LAB",
          "TUE": [[1030, 1200]],
          "FRI": [[1030, 1200]]
        }
      ]
    }|raw};
    let (courses, setCourses) =
      React.useReducer(
        (_, newValue) => newValue,
        courses |> Js.Json.parseExn |> Courses.of_json,
      );

    let handleSubmit =
      React.useCallback1(
        _ => {
          let _ =
            Js.Promise.(
              analyze(input)
              |> then_(courses => {
                   let courses = courses |> Courses.of_json;
                   setCourses(courses);
                   Js.Promise.resolve();
                 })
            );
          let schedule = [|input|];
          setSchedule(schedule);
        },
        [|input|],
      );

    <div className="flex flex-col w-screen h-screen">
      <div className="mx-auto max-w-4xl"> <Courses courses /> </div>
      <div className="w-full h-96">
        <textarea
          className="w-full h-full"
          value=input
          onChange={event =>
            React.Event.Form.target(event)##value |> setInput
          }
        />
      </div>
      <div>
        <button onClick=handleSubmit> {"Submit" |> React.string} </button>
      </div>
      <div> {Js.Array.join(~sep=", ", schedule) |> React.string} </div>
    </div>;
  };
};

[@react.component]
let make = () => {
  <MainApp />;
};
