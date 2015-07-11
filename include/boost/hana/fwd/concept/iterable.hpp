/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_ITERABLE_HPP
#define BOOST_HANA_FWD_CONCEPT_ITERABLE_HPP

#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/back.hpp>
#include <boost/hana/fwd/drop_front.hpp>
#include <boost/hana/fwd/drop_front_exactly.hpp>
#include <boost/hana/fwd/drop_while.hpp>
#include <boost/hana/fwd/front.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/tail.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Iterable` concept represents data structures supporting external
    //! iteration.
    //!
    //! Intuitively, an `Iterable` can be seen as a kind of container whose
    //! elements can be pulled out one at a time. An `Iterable` also provides
    //! a way to know when the _container_ is empty, i.e. when there are no
    //! more elements to pull out.
    //!
    //! Whereas `Foldable` represents data structures supporting internal
    //! iteration with the ability to accumulate a result, the `Iterable`
    //! concept allows inverting the control of the iteration. This is more
    //! flexible than `Foldable`, since it allows iterating over only some
    //! part of the structure. This, in turn, allows `Iterable` to work on
    //! infinite structures, while trying to fold such a structure would
    //! never finish.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `at`, `tail` and `is_empty`
    //!
    //!
    //! @anchor Iterable-lin
    //! The linearization of an `Iterable`
    //! ----------------------------------
    //! Intuitively, for an `Iterable` structure `xs`, the _linearization_ of
    //! `xs` is the sequence of all the elements in `xs` as if they had been
    //! put in a (possibly infinite) list:
    //! @code
    //!     linearization(xs) = [x1, x2, x3, ...]
    //! @endcode
    //!
    //! The `n`th element of the linearization of an `Iterable` can be
    //! accessed with the `at` function. In other words, `at(xs, n) == xn`.
    //!
    //! Note that this notion is precisely the extension of the [linearization]
    //! (@ref Foldable-lin) notion of `Foldable`s to the infinite case.
    //! This notion is useful for expressing various properties of
    //! `Iterable`s, and is used for that throughout the documentation.
    //!
    //!
    //! Compile-time `Iterable`s
    //! ------------------------
    //! A _compile-time_ `Iterable` is an `Iterable` for which `is_empty`
    //! returns a compile-time `Logical`. These structures allow iteration
    //! to be done at compile-time, in the sense that the "loop" doing the
    //! iteration can be unrolled because the total length of the structure
    //! is kown at compile-time.
    //!
    //! In particular, note that being a compile-time `Iterable` has nothing
    //! to do with being finite or infinite. For example, it would be possible
    //! to create a sequence representing the Pythagorean triples as
    //! `IntegralConstant`s. Such a sequence would be infinite, but iteration
    //! on the sequence would still be done at compile-time. However, if one
    //! tried to iterate over _all_ the elements of the sequence, the compiler
    //! would loop indefinitely, in contrast to your program looping
    //! indefinitely if the sequence was a runtime one.
    //!
    //! __In the current version of the library, only compile-time `Iterable`s
    //! are supported.__ While it would be possible in theory to support
    //! runtime `Iterable`s, doing it efficiently is the subject of some
    //! research. In particular, follow [this issue][1] for the current
    //! status of runtime `Iterable`s.
    //!
    //!
    //! Laws
    //! ----
    //! First, we require the equality of two `Iterable`s to be related to the
    //! equality of the elements in their linearizations. More specifically,
    //! if `xs` and `ys` are two `Iterable`s of data type `It`, then
    //! @code
    //!     xs == ys  =>  at(xs, i) == at(ys, i)   for all i
    //! @endcode
    //!
    //! This conveys that two `Iterable`s must have the same linearization
    //! in order to be considered equal.
    //!
    //! Secondly, since every `Iterable` is also a `Searchable`, we require
    //! the models of `Iterable` and `Searchable` to be consistent. This is
    //! made precise by the following laws. For any `Iterable` `xs` with a
    //! linearization of `[x1, x2, x3, ...]`,
    //! @code
    //!     any_of(xs, equal.to(z))  <=>  xi == z
    //! @endcode
    //! for some _finite_ index `i`. Furthermore,
    //! @code
    //!     find_if(xs, pred) == just(the first xi such that pred(xi) is satisfied)
    //! @endcode
    //! or `nothing` if no such `xi` exists.
    //!
    //!
    //! Refined concepts
    //! ----------------
    //! 1. `Searchable` (free model)\n
    //! Any `Iterable` gives rise to a model of `Searchable`, where the keys
    //! and the values are both the elements in the structure. Searching for
    //! a key is just doing a linear search through the elements of the
    //! structure.
    //! @snippet example/iterable.cpp Searchable
    //!
    //! 2. `Foldable` for finite `Iterable`s (free model)\n
    //! Every finite `Iterable` gives rise to a model of  `Foldable`. For these
    //! models to be consistent, we require the models of both `Foldable` and
    //! `Iterable` to have the same linearization.
    //!
    //! @note
    //! As explained above, `Iterable`s are also `Searchable`s and their
    //! models have to be consistent. By the laws presented here, it also
    //! means that the `Foldable` model for finite `Iterable`s has to be
    //! consistent with the `Searchable` model.
    //!
    //! For convenience, a default minimal complete definition for `Foldable`
    //! is provided for finite `Iterable`s via the `Iterable::fold_left_impl`
    //! class. This default implementation uses the fact that `at(xs, i)`
    //! denotes the `i`th element of `xs`'s linearization, and that the
    //! linearization of a finite `Iterable` must be the same as its
    //! linearization as a `Foldable`.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Tuple`, `String`, `Range`
    //!
    //!
    //! [1]: https://github.com/ldionne/hana/issues/40
    struct Iterable {
        template <typename It> struct fold_left_impl;

        template <typename It> struct find_if_impl;
        template <typename It> struct any_of_impl;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONCEPT_ITERABLE_HPP
